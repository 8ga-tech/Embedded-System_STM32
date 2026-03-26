#ifndef __FSM_H
#define __FSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 状态枚举 */
typedef enum {
    STATE_STOP,
    STATE_GPIO_DRIVE,
    STATE_PWM_DRIVE,
    STATE_FORWARD,
    STATE_REVERSE,
    STATE_ERROR
} MotorState;

/* 事件枚举 */
typedef enum {
    EVT_ON,
    EVT_OFF,
    EVT_MODE_GPIO,
    EVT_MODE_PWM,
    EVT_DIR_FORWARD,
    EVT_DIR_REVERSE,
    EVT_SPEED,
    EVT_STATUS,
    EVT_ERROR
} MotorEvent;

/* 函数声明 */
void FSM_HandleCommand(char *cmd);
void FSM_Process(void);
void FSM_TriggerError(void);
#ifdef __cplusplus
}
#endif

#endif /* __FSM_H */

