//
// Created by Alex Neiman on 10/24/23.
//

#ifndef LOCAL_V2_NEURALNET_H
#define LOCAL_V2_NEURALNET_H
#define NUM_LAYERS      2
#define LAYER_SIZE_A    3
#define LAYER_SIZE_B    2
#define NUM_INPUTS      2
#define NUM_OUTPUTS     2
#define NUM_EPOCHS      1000
#define LEARNING_RATE   0.01


#include "../../library/globals.h"
#include "proportional.h"
#include <math.h>
#include <stdlib.h>

// #define DE_DOUT 0

typedef struct
{
    u08 num_weights;
    float weights[3];
    float bias;
    float net;
    float out;
    // float intermediates[4];
    /* data */
} node;

typedef struct {
    node layers[NUM_LAYERS][LAYER_SIZE_A];   // Assuming A is the bigger size
    u08 layer_size[NUM_LAYERS];
} nn;

motor_command compute_neural_network(u08 left, u08 right, nn *network);
void initialize_neural_network(nn *network);
float activation(float input);
void train_neural_network(float *network, float *data, u16 num_data_points);
float compute_derivative(u08 layer, u08 node, u08 weight_no, motor_command input, motor_command output, nn *network);


#endif //LOCAL_V2_NEURALNET_H
