#define SERVER_IP "192.168.0.101" 
#define JSON_BUFFER_SIZE 2048
#define N_SERVOS 12
#define M_PI 3.141592f
#define N_OSC 6 // Number of connected oscillators
#define N_OSC_PLUS_ONE 7
#define N_LEGS 6
#define N_LEGS_PLUS_ONE 7

// see tower pro 90 spec
// Remember SERVO_MIN -> -90deg, SERVO_MAX -> 90deg
// 4096 * 1ms / 20ms this is the 'minimum' pulse length count (out of 4096)
#define SERVO_MIN 205 
// 4096 * 2ms / 20ms this is the 'maximum' pulse length count (out of 4096)
#define SERVO_MAX 510 

#define GAIT_NONE 0 
#define GAIT_METACHRONAL 1
#define GAIT_RIPPLE 2 
#define GAIT_TRIPOD 3

#define MESSAGE_TYPE_SETTINGS 0
#define MESSAGE_TYPE_STEERING 1

