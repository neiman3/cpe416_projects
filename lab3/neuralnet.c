//
// Created by Alex Neiman on 10/24/23.
//

#include "neuralnet.h"



void initialize_neural_network(nn *network) {
    // set default neural network structure
    // input, layers, output size
    uint8_t weights_default_size[4] = {NUM_INPUTS, LAYER_SIZE_A, LAYER_SIZE_B, NUM_OUTPUTS};
    network->layer_size[0] = LAYER_SIZE_A;
    network->layer_size[1] = LAYER_SIZE_B;
    for (u08 l=0; l<NUM_LAYERS; l++){
        // for every layer in the network,
        for (u08 n=0; n<network->layer_size[l]; n++) {
            // for every node in the layer
            node *current_node =  &(network->layers[l][n]); //error: subscripted value is neither array nor pointer nor vector
            // number of weights equal to previous "layer" size
            current_node->num_weights = weights_default_size[l];
            // the node is network.layers[l][n]
            // Alex: -> means depointerize, go to the pointer, and access parameter
            for (u08 w=0; w<current_node->num_weights; w++){
                // for every weight in weights
                current_node->weights[w] = (float) (uint32_t) rand() / (float) RAND_MAX;
            }
            current_node->bias = (float) (uint32_t) rand() / (float) RAND_MAX; // init bias
        }
    }
}

// scale sensor inputs, then calculate a motor command using neural net
motor_command compute_neural_network(u08 left, u08 right, nn *network) {
    float input[2] = {(float) (left - VWL)/(VBL - VWL), (float) (right - VWR)/(VBR - VWR)};
    float output[2] = {0,0};
    motor_command cmd;
    // feed forward into the input layer

    for (u08 l=0; l<NUM_LAYERS; l++) { // for each layer in the network
        for (u08 n=0; n<network->layer_size[l]; n++) {   // each node in layer
            node *current_node = &(network->layers[l][n]); // get current node
            current_node->net = current_node->bias; // initialize bias value to net
            for (u08 w=0; w<current_node->num_weights; w++){
                // for each weight in the node - ie each input
                // for input layer only
                if (l==0) {
                    current_node->net += current_node->weights[w] * input[w]; // use input array
                } else {
                    current_node->net += current_node->weights[w] * network->layers[l-1][w].out; // output value from previous array
                }
            }
            current_node->out = activation(current_node->net);
            if (l == (NUM_LAYERS-1)) {// on the last layer
                output[n] = current_node->out;
            }
        }
    }

    cmd.left = (int8_t) (output[0] * 100);
    cmd.right = (int8_t) (output[1] * 100);

    return cmd;
}

float activation(float input) {
    // Sigmoid activation function
    // (1 + exp(-x)) ^ -1
    return 1 / (1 + (float) expf((float) (-1 * input)));

}


// iterate on training data, adjusting weights after each data point
void train_neural_network(nn *network, sensor_reading *data, u16 num_data_points, u16 num_epochs) {
    // Training data comes in an array of type training_data (holds two uint8_t)
    // NUM_EPOCHS is #define'd
    // num_data_points is num. data points captured in data capture mode
    u16 timer=0;
    for (u16 e=0; e<num_epochs; e++){
        // for each epoch
        #ifndef LOCAL
        lcd_cursor(4,1); print_num(e);
        progress_bar(e, num_epochs, 4, 0, 1, timer, 1);
        #endif
        for (u16 p=0; p<num_data_points; p++) {
            timer++;
            // for each data point in the set:
            sensor_reading *data_point = &(data[p]); // get single data point
            // calculate expected value
            motor_command expected_value = compute_proportional(data_point->left, data_point->right);
            // feed forward with data point
            motor_command network_output = compute_neural_network(data_point->left, data_point->right, network);
            // for each layer in the network, output layer first
            for (int8_t l=1; l>=0; l--) {
                // node *current_layer = &(network->layers[l]); // pointer to the current layer, a array of nodes of size LAYER_SIZE_A
                for (u08 n=0; n<network->layer_size[l]; n++) {
                    
                    // for each node in the layer
                    node *current_node = &(network->layers[l][n]);
                    for (u08 w=0; w<current_node->num_weights; w++) {
                        // for each weight in the node:
                        float derivative = compute_derivative(l, n, w, data_point, &expected_value, &network_output, network);
                        // find derivative
                        // adjust by learning rate in correct sign
                        float old_weight = current_node->weights[w];
                        current_node->new_weights[w] = old_weight - (float) (LEARNING_RATE * derivative);
                    }
                    // for the bias in the node
                    current_node->new_bias = current_node->bias - (float) (LEARNING_RATE / 10 * compute_derivative(l, n, 255, data_point, &expected_value, &network_output, network));
                    // find derivative
                    // adjust by learning rate in correct sign
                }
                update_weights(network);
            }
        }
    }
    // Now transfer new weights to old weights
    for (u08 l=0; l<NUM_LAYERS; l++) {
        node *current_layer = (network->layers[l]); // pointer to the current layer, a array of nodes of size LAYER_SIZE_A
        for (u08 n = 0; n < network->layer_size[l]; n++) {
            // for each node in the layer
            node *current_node = &(current_layer[n]);
            for (u08 w = 0; w < current_node->num_weights; w++) {
                current_node->weights[w] = current_node->new_weights[w];
            }
            current_node->bias = current_node->new_bias;
        }
    }
}

void update_weights(nn *network) {// Now transfer new weights to old weights
    for (u08 l=0; l<NUM_LAYERS; l++) {
        node *current_layer = (network->layers[l]); // pointer to the current layer, a array of nodes of size LAYER_SIZE_A
        for (u08 n = 0; n < network->layer_size[l]; n++) {
            // for each node in the layer
            node *current_node = &(current_layer[n]);
            for (u08 w = 0; w < current_node->num_weights; w++) {
                current_node->weights[w] = current_node->new_weights[w];
            }
            current_node->bias = current_node->new_bias;
        }
    }
}

float compute_derivative(u08 layer, u08 node, u08 weight_no, sensor_reading *input, motor_command *input_target, motor_command *output, nn *network) {
    // pass in 255 (-1) as weight_no
    float dEt_dwn; // "slope" for node (resultant value for this function)
//    node *current_node;
//    current_node = &(network->layers[layer][node]); // get current node
    float out = network->layers[layer][node].out;
    float target;
    float input_value;
    (node == 0) ? (target = sinput_target->left) : (target = input_target->right); // get the expected value 0 or 1, left or right
    // The input target should be mapped as a float
    (weight_no == 0) ? (input_value = input->left) : (input_value = input->right); // get the input value 0 or 1, left or right
    if (layer == 1) {
        // OUTPUT LAYER CALCULATION
        // Assume weight_no is 0 or 1
        //              (error)         * (d/do sigmoid)    * (dout / d(weight[w]))[output  of h[w]]
        float dEt_douton = (out - target);
        float douton_dneton = out * (1 - out);
        float dneton_dwn;
        dneton_dwn = (weight_no == 255) ? 1 : (network->layers[layer - 1][weight_no].out);
        dEt_dwn = dEt_douton * douton_dneton * dneton_dwn;

    } else {
        // INPUT LAYER CALCULATION
        float dEt_douthn = 0;    // sum from each output
        for (u08 sum=0; sum<NUM_OUTPUTS; sum++) {
            // for outputs 0 and 1, sum 
            float dEon_douton=(out - target);
            float douton_dneton = (out * (1-out));
            float dneton_douthn;
            dneton_douthn = (weight_no == 255) ? 1 : (network->layers[1][sum].weights[node]);
            dEt_douthn += dEon_douton * douton_dneton * dneton_douthn;
        }
        float dnethn_dwn = input_value;
        float douthn_dnethn = network->layers[0][node].out;
        dEt_dwn = dEt_douthn * douthn_dnethn * dnethn_dwn;
        
    }

    return dEt_dwn;
}

//void debug(char *string, u16 value) {
//    lcd_cursor(0,0); print_string(string);
//    lcd_cursor(0,1); print_num(value);
//    _delay_ms(2000);
//}