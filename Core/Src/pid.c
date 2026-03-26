#include "pid.h"
#include <math.h>

/* PID初始化 */
void PID_Init(PID_HandleTypeDef *pid, float kp, float ki, float kd)
{
  if(pid == NULL) return;
  /* 修正：实际赋值，消除未引用警告 */
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;
  pid->integral = 0.0f;
  pid->prev_error = 0.0f;
  pid->output = 0.0f;
}

/* PID更新 */
float PID_Update(PID_HandleTypeDef *pid, float setpoint, float measurement)
{
  if(pid == NULL) return 0.0f;
  
  float error = setpoint - measurement;
  /* 积分项 */
  pid->integral += error * 0.001f; // 假设1ms周期
  /* 微分项 */
  float derivative = (error - pid->prev_error) / 0.001f;
  /* PID输出 */
  pid->output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
  
  /* 限幅（示例） */
  if(pid->output > 1000) pid->output = 1000;
  if(pid->output < -1000) pid->output = -1000;
  
  pid->prev_error = error;
  return pid->output;
}

/* PID重置 */
void PID_Reset(PID_HandleTypeDef *pid)
{
  if(pid == NULL) return;
  pid->integral = 0.0f;
  pid->prev_error = 0.0f;
  pid->output = 0.0f;
}

