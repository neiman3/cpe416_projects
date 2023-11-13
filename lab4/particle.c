#include "particle.h"

// Map: floor an input integer between two values and linearly interpolate it to the specified range
// example: map(input=10, input_range_min=0, input_range_max=20, output_range_min=0 float output_range_max=1)
//          |--->  0.5
float map(uint16_t input, uint16_t input_range_min, uint16_t input_range_max, float output_range_min, float output_range_max) {
    uint16_t input_floor;
    if (input < input_range_min) {
        input_floor = input_range_min;
    } else if (input > input_range_max) {
        input_floor = input_range_max;
    } else {
        input_floor = input;
    }

    return ((float) (input_floor - input_range_min) / (float) (input_range_max - input_range_min)) * (output_range_max - output_range_min) + output_range_min;
}

// Wrap a 0xffff (32 bit) fixed point numer to 360º
float fixed_point_pos_to_float(uint16_t data) {
    return map(data, MIN_POSITION, MAX_POSITION, 0, 360);
}
uint16_t float_to_fixed_point_pos(float data) {
    return (uint16_t) (data * MN_CONV_FACTOR); // magic number = 50,000 / 360.0
}

// initialize particles randomly. only run once
// make a random distribution of particles around a 360º circle
void init_particles(particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers) {
    //float noise;
    float pos;

    for(int i=0; i<num_particles; i++) {
        // not currently using noise
        pos = map(i, 0, num_particles, 0, 360);
        data[i].position = float_to_fixed_point_pos(pos);
        data[i].weight = (float) 1.0 / (float) num_particles;
    }
}

// Generate a set of new particles with most of them centered around the previous highly weighted particles
void resample(particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers) {
    // TODO: Test
    // Normalize particle weights before start
    normalize_particle_weights(data, num_particles);
    // Sort particles from high to low by weight
//    sort_particles(data, num_particles, 0, num_particles-1);
    // Temp: use bubble sort to sort
    bubble_sort(data, num_particles);
    // Insert particles proportional to their weight, replacing lowest particles
    uint8_t counter=0;
    int16_t num_inserts;
    // Particle points to start and end of list.
    uint16_t read_index = 0;
    uint16_t insertion_index = num_particles - 1;

    // Calculate number of inserts minus itself
    num_inserts = ((int16_t) (data[counter].weight * (float) num_particles)); // number of times to insert the new particle (by weight)
    if (num_inserts > 0) { num_inserts -= 1; }
    while (read_index < insertion_index) {
        for (int16_t i = 0; i < num_inserts; i++) {
            data[insertion_index] = data[read_index];
            insertion_index--;
        }
        read_index++;
    }


    // normalize
    normalize_particle_weights(data, num_particles);

    // Add 5% random particles
    uint8_t random_index;
    float random_position;
    for (uint8_t i=0; i<(uint8_t) ((uint16_t) (NUM_PARTICLES * NUM_RANDOM_INSERTS / 100)); i++) {
        // pick a random particle and seed it with uniform weight, random position
        random_index = (uint8_t) (NUM_PARTICLES * (float) (uint32_t) rand() / (float) RAND_MAX);
        random_position = (float) (((float) (uint32_t) rand() / (float) RAND_MAX) * 360.0);
        data[random_index].weight = (float) 1.0 / (float) NUM_PARTICLES;
        data[random_index].position = float_to_fixed_point_pos(random_position);
    }

    // normalize finally, again (since we inserted new particles and weights)
    normalize_particle_weights(data, num_particles);
}

// Quick sort implementation to sort particles by their weights
void sort_particles(particle *data, uint8_t num_particles, uint8_t start, uint8_t stop) {
    uint8_t pivot;
    if(num_particles <= 1 || start >= stop)
        return;
    pivot = partition(data, start, stop);
    sort_particles(data, num_particles, start, pivot - 1);
    sort_particles(data, num_particles, pivot + 1, stop);
}

// helper for sort_particles()
uint8_t partition(particle *data, uint8_t start, uint8_t end) {
    uint8_t pivot_i = start;
    float pivot_wt = data[end].weight;
    for(u08 i=start; i<end; i++) {
        if(data[i].weight > pivot_wt) {
            swap_particles(&data[pivot_i], &data[i]);
            pivot_i++;
        }
    }
    return pivot_i - 1;
}

void bubble_sort(particle *data, uint8_t num_particles) {
    uint8_t index = 0;
    particle t;
    while ((index-1) < num_particles) {
        if (data[index].weight >= data[index+1].weight) {
            // in order, continue
            index++;
        } else {
            // swap
            swap_particles(&data[index], &data[index+1]);
            index = 0;
        }
    }
}

// Takes an array of sorted particles and a pointer to a single particle.
// Inserts a duplicate particle after the target, overwriting the last particle.
void duplicate_particle(particle *data, uint8_t num_particles, particle *target_position) {
    // Start from bottom of the list
    for (int i=num_particles-1; i>0; i--) {
        // Replace current particle with particle ahead of it
        data[i] = data[i-1];
        // Look ahead to next particle and see if it is our target particle
        if ((data + i - 1) == target_position) { // Do the pointers match?
            return; // exit now
        }
    }
}

// swap the values of particles x and y
void swap_particles(particle *x, particle *y) {
    particle n;
    n.position = x->position;
    n.weight = x->weight;
    x->position = y->position;
    x->weight = y->weight;
    y->position = n.position;
    y->weight = n.weight;
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
        pos = fixed_point_pos_to_float(data[i].position); // convert stored uint16 to float degrees
        pos += (float) position_delta * (float) 0.7397667; // add the robot position delta (converted to degrees) to particle position
        pos = add_noise(pos, (float) 0.013909689); // Apply noise based on the motion model
        data[i].position = float_to_fixed_point_pos(wrap_degrees(pos)); // Wrap degrees (0-360) and convert back to uint16. Restore
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
    return result;
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
void calculate_sensor_probability(uint8_t sensor_reading, particle *data, uint8_t num_particles, tower *tower_positions, uint8_t num_towers) {
    float p_tower = map(sensor_reading, DIST_THRESHOLD_LOW, DIST_THRESHOLD_HIGH, 0, 1); // probability of there being a tower currently at the sensor reading
    for (uint8_t i=0; i<num_particles; i++) {
        float expectation = calculate_position_probability(data[i].position, tower_positions, num_towers);
        // expectation is expected sensor probability
        data[i].weight += add_noise( WEIGHT_CONSTANT * expectation * p_tower , (float) 0.0001);
    }
}

// Wrap an angular position around so that it is always given in positive degree angle of one full rotation (0-360)
float wrap_degrees(float data) {
    while(data < 0) {
        data += 360;
    }
    while(data > 360) {
        data -= 360;
    }
    return data;
}

// Normalize weights.
void normalize_particle_weights(particle *data, uint8_t num_particles) {
    float sum=0;
    for (uint8_t i=0;i<num_particles;i++){
        sum += data[i].weight;
    }
    for (uint8_t i=0;i<num_particles;i++){
        data[i].weight = data[i].weight / sum;
    }
}


float mean(particle *data, uint8_t num_particles) {
    // TODO: Write
    return 0;
}

float stdev(particle *data, uint8_t num_particles) {
    // TODO: Write
    return 0;
}

void mean_st_dev(particle *data, uint8_t num_particles, float *mean, float *st_dev) {
    // Have to use cartesian representation since directional mean requires it
    float mean_result_real = 0;
    float mean_result_cplx = 0;
    float angle;
    for (int i = 0; i < num_particles; i++) {
        // x = 1exp(jθ)
        // so real part cos(θ) and imag sin(θ)
        angle = fixed_point_pos_to_float(data[i].position) * (float) (M_PI / 180);
        mean_result_real += cosf(angle) / (float) num_particles;
        mean_result_cplx += sinf(angle) / (float) num_particles;
    }
    // calculate variance
    float st_dev_result_real = 0;
    float st_dev_result_cplx = 0;
    for (int i = 0; i < num_particles; i++) {
        // variance = ∑ (x-µ)^2 / N
        angle = fixed_point_pos_to_float(data[i].position) * (float) (M_PI / 180);
        st_dev_result_real += powf((cosf(angle) - mean_result_real), 2) / (float) num_particles;
        st_dev_result_cplx += powf((sinf(angle) - mean_result_cplx), 2) / (float) num_particles;
    }
    st_dev_result_real = sqrtf(st_dev_result_real);
    st_dev_result_cplx = sqrtf(st_dev_result_cplx);

    // Mean is the angle of the real to cplx
    *mean = atanf(mean_result_cplx / mean_result_real) * (float) (180 / M_PI);
    // Geometric sum
    *st_dev = sqrtf(st_dev_result_real * st_dev_result_real + st_dev_result_cplx * st_dev_result_cplx);
}