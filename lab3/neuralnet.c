#include "neuralnet.h"
#include <math.h>
#include <stdlib.h>

void initialize_neural_network(nn *network) {
    // set default neural network structure
    network->layer_size[0] = LAYER_SIZE_A;
    network->layer_size[1] = LAYER_SIZE_B;
    for (u08 l=0; l<NUM_LAYERS; l++){
        // for every layer in the network,
        for (u08 n=0; n<network->layer_size[l]; n++) {
            // for every node in the layer
            node *current_node =  &(network->layers[l][n]); //error: subscripted value is neither array nor pointer nor vector
            // the node is network.layers[l][n]
            // Alex: -> means depointerize, go to the pointer, and access parameter
            for (u08 w=0; w<current_node->num_weights; w++){
                // for every weight in weights
                current_node->weights[w] = rand() / RAND_MAX;
            }
            current_node->bias = rand() / RAND_MAX; // init bias
        }
    }
}

// scale sensor inputs, then calculate a motor command using neural net
motor_command compute_neural_network(u08 left, u08 right, nn *network) {
    float input[2] = {(left - VWL)/(VBL - VWL), (right - VWR)/(VBR - VWR)};
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

    cmd.left = output[0];
    cmd.right = output[1];
    
    return cmd;
}

float activation(float input) {
    // Sigmoid activation function
    // (1 + exp(-x)) ^ -1
    return 1 / (1 + expf(-1 * input));

} 


// iterate on training data, adjusting weights after each epoch
void train_neural_network(float *network, float *data) {
    return;
    
}