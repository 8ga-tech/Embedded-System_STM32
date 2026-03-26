#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float prev_error;
    float output;
} PID_HandleTypeDef;

void PID_Init(PID_HandleTypeDef *pid, float kp, float ki, float kd);
float PID_Update(PID_HandleTypeDef *pid, float setpoint, float measurement);
void PID_Reset(PID_HandleTypeDef *pid);

#ifdef __cplusplus
}
#endif

#endif /* __PID_H */

