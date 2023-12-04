#ifndef PWM_H
#define PWM_H

void PWM_Init(void);
void PWM_SetDutyCycle(char* pwm, unsigned int dutyCycle);
void PWM_Close(void);

#endif // PWM_H