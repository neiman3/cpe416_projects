#ifndef NEURALNET_H
#define NEURALNET_H

#define NUM_LAYERS 2
#define LAYER_SIZE_A 3
#define LAYER_SIZE_B 2
#define NUM_INPUTS 2
#define NUM_OUTPUTS 2
#define NUM_EPOCHS 1000


#include "../library/globals.h"
#include "proportional.h"
#include <math.h>
#include <stdlib.h>

typedef struct
{
    u08 num_weights;
    float weights[3];
    float bias;
    float net;
    float out;
    /* data */
} node;

typedef struct {
    node layers[NUM_LAYERS][LAYER_SIZE_A];   // Assuming A is the bigger size
    u08 layer_size[NUM_LAYERS];
} nn;

motor_command compute_neural_network(u08 left, u08 right, nn *network);
void initialize_neural_network(nn *network);
float activation(float input);
void train_neural_network(float *network, float *data);
#endif