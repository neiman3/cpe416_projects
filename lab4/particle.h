//
// Created by Alex Neiman and Beck Dehlsen on 11/8/23.
//


#define LOCAL

#ifndef PARTICLE_H
#define PARTICLE_H
#include "../library/globals.h"
#include <math.h>
#include <stdlib.h>
#include "proportional.h"
#ifdef LOCAL
#include <stdio.h>
#endif

#define MAX_NUM_TOWERS  4
#define DIST_THRESHOLD_LOW 38
#define DIST_THRESHOLD_HIGH 100
#define TOWER_EDGE_DISTANCE 5
//particle's position on the circle, ~16 bit precision
#define MIN_POSITION (uint16_t) 0
#define MAX_POSITION (uint16_t) 50000
#define NUM_PARTICLES 100
#define WEIGHT_CONSTANT 1.0
#define NUM_RANDOM_INSERTS 0
#define LOCALIZED_THRESHOLD

#define MN_CONV_FACTOR ((float) MAX_POSITION / 360.0)

// Tower model
typedef struct
{
    uint16_t position;
    uint8_t target;
    uint8_t active;
} tower;

// Particle model
typedef struct
{
    uint16_t position;
    float weight;
} particle;

void init_particles(particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers);
void resample(particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers);
void motion_update(particle *data, uint8_t num_particles, uint16_t position_delta);
void duplicate_particle(particle *data, uint8_t num_particles, particle *target_position);
void sort_particles(particle *data, uint8_t start, uint8_t stop);
void normalize_particle_weights(particle *data, uint8_t num_particles);
uint8_t partition(particle *data, uint8_t start, uint8_t end);
void swap_particles(particle *x, particle *y);
float map(uint16_t input, uint16_t input_range_min, uint16_t input_range_max, float output_range_min, float output_range_max);
float add_noise(float input_value, float stdev);
float calculate_position_probability(float particle_position, tower *tower_positions, uint8_t number_of_towers);
void calculate_sensor_probability(uint8_t sensor_reading, particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers);
float fixed_point_pos_to_float(uint16_t data);
float trapezoidal_pdf(float theta_read, float theta_tower);
float wrap_degrees(float data);
void mean_st_dev(particle *data, uint8_t num_particles, float *mean, float *st_dev);
uint16_t float_to_fixed_point_pos(float data);

#endif
