/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 2: ANN training
 *
 **/

#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define NUM_WEIGHTS = 17    // number of weights in the NN model, including biases
#define EPOCHS = 1000       // full training cycles

/*** TODO: RECORD DATASET TO TRAIN ON ***/
// iterate on training data, adjusting weights after each epoch
void train_nn(float *weights) {
    return;
}

int main(void) {

    // float nn_weights[NUM_WEIGHTS];   // Weights from neural net training

    motor_command mc = compute_proportional(193, 182);
    
     // init weights pre-training
    for(int i=0; i<NUM_WEIGHTS; i++) {
        nn_weights = 0;
        nn_weights = -1;    // bias "weights"
    }

    // train model
    // train_nn(nn_weights);

    printf("%d", mc.left); printf(" ");
    printf("%d", mc.right);
    printf("\n");


    return 0;
}