#include "../library/globals.h"
#include <math.h>

// Tunings and threhsold
#define VWL 170 // white threshold for left
#define VWR 170 // white threshold for right
#define VBL 212 // black threshold for left
#define VBR 212 // black threshold for right
#define VSTATE_W_W 50 // white on white Vstate threshold
#define VSTATE_B_B 10 // black on black threshold

#define GAIN_KP_NUM 25
#define GAIN_KP_DEN 10

#define GAIN_KI_NUM 10
#define GAIN_KI_DEN 10

#define GAIN_KD_NUM 10
#define GAIN_KD_DEN 10

#define FWD_SPEED 30 // Default forward speed should be 30
#define THETA_FWD 45 // straight angle (zero/forward)
#define HISTORY_LENGTH 10


typedef struct {
    int8_t left;
    int8_t right;

} motor_command;

motor_command proportional(u08 , u08);



float calculate_vstate_vector(u08, u08, u08, u08);
float calculate_theta(u08, u08, u08, u08);
int16_t min(int16_t, int16_t);
int16_t max(int16_t, int16_t);
int16_t bound(int16_t, int16_t, int16_t);
void motor_dir(int16_t, int8_t);
void motor(uint8_t num, int8_t speed);
void time_advance(u16, u16);
int16_t proportional_error(u16, u16);
int16_t integral_error(u16, u16);
int16_t derivative_error(u16, u16);
motor_command compute_proportional(u08, u08);