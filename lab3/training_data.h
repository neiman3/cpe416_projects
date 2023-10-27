//
// Created by Alex Neiman on 10/27/23.
//

#ifndef CODE_TRAINING_DATA_H
#define CODE_TRAINING_DATA_H

#include "neuralnet.h"
#define NUM_TRAINING_POINTS 79

sensor_reading data[107] = {
        { .left=188, .right=176},
        { .left=188, .right=179},
        { .left=190, .right=176},
        { .left=185, .right=177},
        { .left=184, .right=180},
        { .left=190, .right=183},
        { .left=184, .right=181},
        { .left=180, .right=181},
        { .left=189, .right=178},
        { .left=186, .right=178},
        { .left=186, .right=182},
        { .left=184, .right=175},
        { .left=181, .right=179},
        { .left=181, .right=174},
        { .left=186, .right=184},
        { .left=188, .right=185},
        { .left=181, .right=182},
        { .left=181, .right=177},
        { .left=178, .right=181},
        { .left=179, .right=184},
        { .left=183, .right=183},
        { .left=185, .right=184},
        { .left=188, .right=183},
        { .left=185, .right=183},
        { .left=187, .right=184},
        { .left=177, .right=180},
        { .left=187, .right=180},
        { .left=185, .right=180},
        { .left=180, .right=178},
        { .left=178, .right=185},
        { .left=187, .right=183},
        { .left=178, .right=178},
        { .left=186, .right=177},
        { .left=183, .right=183},
        { .left=179, .right=181},
        { .left=181, .right=181},
        { .left=182, .right=177},
        { .left=180, .right=177},
        { .left=182, .right=182},
        { .left=181, .right=183},
        { .left=183, .right=185},
        { .left=177, .right=181},
        { .left=176, .right=181},
        { .left=180, .right=181},
        { .left=176, .right=180},
        { .left=175, .right=185},
        { .left=180, .right=178},
        { .left=177, .right=187},
        { .left=184, .right=179},
        { .left=176, .right=180},
        { .left=175, .right=187},
        { .left=178, .right=186},
        { .left=184, .right=179},
        { .left=181, .right=187},
        { .left=183, .right=184},
        { .left=183, .right=185},
        { .left=178, .right=184},
        { .left=180, .right=181},
        { .left=175, .right=184},
        { .left=176, .right=183},
        { .left=175, .right=188},
        { .left=180, .right=185},
        { .left=180, .right=190},
        { .left=180, .right=189},
        { .left=177, .right=185},
        { .left=174, .right=182},
        { .left=176, .right=189},
        { .left=177, .right=186},
        { .left=181, .right=180},
        { .left=176, .right=186},
        { .left=175, .right=188},
        { .left=181, .right=190},
        { .left=182, .right=182},
        { .left=178, .right=186},
        { .left=179, .right=182},
        { .left=173, .right=184},
        { .left=177, .right=182},
        { .left=178, .right=187},
        { .left=183, .right=190},
        { .left=190, .right=176},
        { .left=188, .right=179},
        { .left=195, .right=173},
        { .left=187, .right=171},
        { .left=189, .right=181},
        { .left=195, .right=178},
        { .left=184, .right=175},
        { .left=187, .right=177},
        { .left=185, .right=174},
        { .left=184, .right=174},
        { .left=189, .right=173},
        { .left=185, .right=180},
        { .left=185, .right=173},
        { .left=181, .right=174},
        { .left=186, .right=175},
        { .left=185, .right=175},
        { .left=180, .right=180},
        { .left=187, .right=177},
        { .left=189, .right=184},
        { .left=184, .right=176},
        { .left=183, .right=182},
        { .left=183, .right=181},
        { .left=181, .right=175},
        { .left=177, .right=180},
        { .left=186, .right=185},
        { .left=178, .right=177},
        { .left=179, .right=183},
        { .left=176, .right=177}
        };// left-right training data


#endif //CODE_TRAINING_DATA_H
