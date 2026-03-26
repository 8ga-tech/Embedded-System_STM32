#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void Motor_SetPWM(int16_t pwm);
void Motor_Stop(void);
void Motor_SetDirection(uint8_t dir);  // 0:停, 1:正转, 2:反转
void Motor_SetMode(uint8_t mode);      // 0:GPIO, 1:PWM
int16_t Motor_GetPWM(void);            // 获取当前PWM值

#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H */
