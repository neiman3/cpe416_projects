//
// Created by Alex Neiman on 10/24/23.
//

#include "neuralnet.h"
#include "proportional.h"

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
void train_neural_network(float *network, training_data *data, u16 num_data_points) {
    // Training data comes in an array of type training_data (holds two uint8_t)
    // NUM_EPOCHS is #define'd
    // num_data_points is num. data points captured in data capture mode
    for (u16 e=0; e<NUM_EPOCHS; e++){
        // for each epoch
        for (u16 p=0; p<num_data_points; p++) {
            // for each data point in the set:

            // feed forward with data point
            // for each layer in the network, output layer first
                // for each node in the layer
                    // for each weight in the node
                        // find derivative
                        // adjust by learning rate in correct sign
                    // for the bias in the node
                        // find derivative
                        // adjust by learning rate in correct sign
        }
    }
}

//void debug(char *string, u16 value) {
//    lcd_cursor(0,0); print_string(string);
//    lcd_cursor(0,1); print_num(value);
//    _delay_ms(2000);
//}