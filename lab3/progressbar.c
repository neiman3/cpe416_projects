#include "progressbar.h"
#ifndef LOCAL
void progress_bar(uint16_t current, uint16_t max, u08 display_width,  u08 display_column, u08 display_row, u08 timer, u08 option_animation) {
    u08 num_chars = (uint8_t) ((uint32_t) current * display_width / max);
    lcd_cursor(display_column, display_row);
    for (int i=0; i<num_chars; i++){
        print_string("=");
    }
    if (option_animation) {
        // scroll - \ | / 
        if (timer < 63) {
            print_string("-");
        } else if (timer < 127) {
            print_string("`");
        } else if (timer < 191) {
            print_string("|");
        } else {
            print_string("/");
        }
    }
    for (int i=0; i<display_width - num_chars - 1; i++) {
        print_string(" ");
    }

}
#endif