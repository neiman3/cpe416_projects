/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 2: Neural network line following. 
 *     
 *      Neural network library header. Contains structures for network.
 *
 **/

#ifndef LOCAL_V2_NEURALNET_H
#define LOCAL_V2_NEURALNET_H
#define NUM_LAYERS      2
#define LAYER_SIZE_A    3
#define LAYER_SIZE_B    2
#define NUM_INPUTS      2
#define NUM_OUTPUTS     2
#define MIN_EPOCHS      1
#define MAX_EPOCHS     50
#define LEARNING_RATE   0.15
// #define LOCAL

#include "../library/globals.h"
#include "proportional.h"
#include <math.h>
#include <stdlib.h>
#include "proportional.h"
#include "progressbar.h"
#ifdef LOCAL
#include <stdio.h>
#endif


// #define DE_DOUT 0

typedef struct
{
    u08 num_weights;
    float weights[3];
    float new_weights[3];
    float new_bias;
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
void train_neural_network(nn *network, sensor_reading *data, u16 num_data_points, u16 num_epochs);
float compute_derivative(u08 layer, u08 node, u08 weight_no, sensor_reading *input, motor_command *input_target, motor_command *output, nn *network);
void update_weights(nn *network);

#endif //LOCAL_V2_NEURALNET_H
