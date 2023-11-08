#include "particle.h"

float map(int16_t input, int16_t input_range_min, int16_t input_range_max, float output_range_min, float output_range_max) {
    // Map: floor an input integer between two values and linearly interpolate it to the specified range
    // example: map(input=10, input_range_min=0, input_range_max=20, output_range_min=0 float output_range_max=1)
    //          |--->  0.5
    uint16_t input_floor;
    if (input < input_range_min) {
        input_floor = input_range_min;
    } else if (input > input_range_max){
        input_floor = input_range_max;
    } else {
        input_floor = input;
    }

    return ((float) (input_floor - input_range_min) / (float) (input_range_max - input_range_min)) * (output_range_max - output_range_min) + output_range_min;
}

// Wrap a 0xffff
float fixed_point_pos_to_float(uint16_t data) {
    return map((int16_t) data, MIN_POSITION, MAX_POSITION, 0, 360);
}
uint16_t float_to_fixed_point_pos(float data) {
    return (uint16_t) (data * MN_CONV_FACTOR); // magic number = 50,000 / 360.0
}

// initialize particles randomly. only run once
// make a random distribution of particles around a 360º circle
void init_particles(particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers) {
    float noise;
    float pos;

    for(int i=0; i<num_particles; i++) {
        // not currently using noise
        pos = map((int16_t) i, 0, num_particles, 0, 360);
        data[i].position = float_to_fixed_point_pos(pos);
        data[i].weight = float_to_fixed_point_pos((float) 1.0 / (float) num_particles);
        data[i].expectation = calculate_position_probability(pos, tower_positions, num_towers);
    }
}

// Generate a set of new particles with most of them centered around the previous highly weighted particles
void resample(particle *data, uint8_t num_particles) {
    // TODO: Write function
    sort_particles(data, num_particles);
}

void sort_particles(particle *data, uint8_t num_particles) {
    
}

// Advance all particles based on known position movement of the robot. 
// Position delta is given in ticks.
// Apply some noise based on the motion model. 
void motion_update(particle *data, uint8_t num_particles, uint16_t position_delta) {
    // Radius of circle: 10 inch
    // Radius of travel of outer wheel: 13 inch
    // Circumferential travel of outer wheel: 81.7 inch
    // Number of ticks per inch: 5.96 (σ=0.113)
    // Number of ticks per degree around outer circle: 4.4

    // Number of degrees per tick: 0.739 (σ=0.0139)
    // Number of ticks per degree: 1.35 (σ=0.0256)
    float pos=0;
    for (int i=0; i<num_particles; i++){
        // for each particle:
        pos = data[i].position
    }




}

// Use Box-Mueller transform to add gaussian noise with a given mean and standard deviation
float add_noise(float input_value, float stdev) {
    float random1 = (float) (uint32_t) rand() / (float) RAND_MAX;
    float random2 = (float) (uint32_t) rand() / (float) RAND_MAX;
    float z = sqrtf(-2 * logf(random1)) * cosf(2 * M_PI * random2);
    // Z is normalized where X = μ + σZ
    return input_value + z * stdev;
}

// Calculate the likelihood of a position given a probability of a tower
float calculate_position_probability(float particle_position, tower *tower_positions, uint8_t number_of_towers) {
    float result = 0;
    for (int i=0; i< number_of_towers; i++){
        result += trapezoidal_pdf(particle_position, fixed_point_pos_to_float(tower_positions[i].position));
    }
    if (result > 1) {
        result = 1; // just in case there are improperly defined tower locations that are not disjoint and P>1
    }
    
}

// Trapezoidal approximation
float trapezoidal_pdf(float theta_read, float theta_tower) {
    // Constants can be adjusted here
    float a = -15; // ±5º is the slip range
    float b = -10; // degrees, equal to arctangent of 1/2 wooden block size / block radius
    float c = -1 * b;
    float d = -1 * a;
    float theta = theta_read - theta_tower;
    float u = 2 / (a + c - b - a);

    if (theta < a) {
        return 0;
    } else if (theta < b) {
        return u * (theta - a) / (b - a);
    } else if (theta < c) {
        return u;
    } else if (theta < d) {
        return u * (d - theta) / (d - c);
    } else {
        return 0;
    }
}

// Calculate the likelyhood of a tower given a sensor reading
float calculate_sensor_probability(uint8_t sensor_reading, particle *data) {
    
}

// Wrap an angular position around so that it is always given in positive degree angle of one full rotation (0-360)
float wrap_degrees(float data) {

}