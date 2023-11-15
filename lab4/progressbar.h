// Header for progressbar.c
//  #define LOCAL

#ifndef _PROGRESSBAR_H
#define _PROGRESSBAR_H
#include "../library/globals.h"

#ifndef LOCAL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include <math.h>
#include <stdlib.h>

void progress_bar(uint16_t current, uint16_t max, u08 display_width, u08 display_row, u08 display_column, u08 timer, u08 option_animation);

#endif
