#include <Servo.h>

#define SIG_SERVO 3

Servo servo;

// initialise le servo
void initServo(void) {
  servo.attach(SIG_SERVO);
}

// deplace le servo pour qu'il soit dans l'angle donee
void turn(int angle) {
  servo.write(180-angle);
}