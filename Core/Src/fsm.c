#include "main.h"
#include "fsm.h"
#include "usart.h"
#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include <string.h>
#include <stdlib.h>

// 外部变量声明
extern ControlMode control_mode;
extern PID_HandleTypeDef pid;
extern uint8_t pid_enabled;
extern float target_rpm;

static MotorState current_state = STATE_STOP;
static void FSM_DispatchEvent(MotorEvent evt, int param);






void FSM_HandleCommand(char *cmd) {
    if (strcmp(cmd, "ON") == 0) {
        FSM_DispatchEvent(EVT_ON, 0);
    } else if (strcmp(cmd, "OFF") == 0) {
        FSM_DispatchEvent(EVT_OFF, 0);
    } else if (strncmp(cmd, "SPEED ", 6) == 0) {
        int speed = atoi(cmd + 6);
        if (speed < 0 || speed > 100) {
            UART_SendString("ERROR: speed out of range\r\n");
        } else {
            FSM_DispatchEvent(EVT_SPEED, speed);
        }
    } else if (strcmp(cmd, "MODE GPIO") == 0) {
        FSM_DispatchEvent(EVT_MODE_GPIO, 0);
    } else if (strcmp(cmd, "MODE PWM") == 0) {
        FSM_DispatchEvent(EVT_MODE_PWM, 0);
    } else if (strcmp(cmd, "DIR FORWARD") == 0) {
        FSM_DispatchEvent(EVT_DIR_FORWARD, 0);
    } else if (strcmp(cmd, "DIR REVERSE") == 0) {
        FSM_DispatchEvent(EVT_DIR_REVERSE, 0);
    } else if (strcmp(cmd, "STATUS") == 0) {
        FSM_DispatchEvent(EVT_STATUS, 0);
    } else if (strncmp(cmd, "SET_RPM ", 8) == 0) {
        int rpm = atoi(cmd + 8);
        if (rpm < 0 || rpm > 1000) {
            UART_SendString("ERROR: RPM out of range\r\n");
        } else {
            extern ControlMode control_mode;
            extern PID_HandleTypeDef pid;
            extern uint8_t pid_enabled;
            target_rpm = (float)rpm;
            pid_enabled = 1;
            control_mode = MODE_PID_CLOSED;
            PID_Reset(&pid);
            UART_SendString("OK: PID mode, target RPM set\r\n");
        }
    } else if (strcmp(cmd, "RPM") == 0) {
        char buf[32];
        float real_rpm = Encoder_GetRPM();
        sprintf(buf, "Current RPM: %.0f\r\n", real_rpm);
        UART_SendString(buf);
    } else if (strcmp(cmd, "AUTO") == 0) {
        extern ControlMode control_mode;
        extern uint8_t pid_enabled;
        control_mode = MODE_AUTO_PWM;
        pid_enabled = 0;
        UART_SendString("OK: Auto PWM mode\r\n");
    } else {
        UART_SendString("ERROR: unknown command\r\n");
    }
}

static void FSM_DispatchEvent(MotorEvent evt, int param) {
    if (current_state == STATE_ERROR && evt != EVT_ON) {
        UART_SendString("ERROR: must send ON to recover\r\n");
        return;
    }

    if (evt == EVT_ERROR) {
        if (current_state != STATE_ERROR) {
            current_state = STATE_ERROR;
            Motor_Stop();
            UART_SendString("ERROR: motor stalled, send ON to recover\r\n");
        }
        return;
    }

    switch (current_state) {
        case STATE_STOP:
            if (evt == EVT_ON) {
                current_state = STATE_PWM_DRIVE;
                Motor_SetMode(1);
                Motor_SetDirection(1);
                Motor_SetPWM(200);
                UART_SendString("OK: motor started\r\n");
            } else if (evt == EVT_MODE_GPIO) {
                current_state = STATE_GPIO_DRIVE;
                Motor_SetMode(0);
                UART_SendString("OK: GPIO mode\r\n");
            } else if (evt == EVT_MODE_PWM) {
                current_state = STATE_PWM_DRIVE;
                Motor_SetMode(1);
                UART_SendString("OK: PWM mode\r\n");
            } else {
                UART_SendString("ERROR: invalid event in STOP state\r\n");
            }
            break;

        case STATE_PWM_DRIVE:
            if (evt == EVT_OFF) {
                current_state = STATE_STOP;
                Motor_Stop();
                UART_SendString("OK: motor stopped\r\n");
            } else if (evt == EVT_SPEED) {
                int pwm_val = param * 10;
                if (pwm_val > 1000) pwm_val = 1000;
                Motor_SetPWM(pwm_val);
                UART_SendString("OK: speed set\r\n");
            } else if (evt == EVT_DIR_FORWARD) {
                Motor_SetDirection(1);
                UART_SendString("OK: forward\r\n");
            } else if (evt == EVT_DIR_REVERSE) {
                Motor_SetDirection(2);
                UART_SendString("OK: reverse\r\n");
            } else if (evt == EVT_MODE_GPIO) {
                current_state = STATE_GPIO_DRIVE;
                Motor_SetMode(0);
                UART_SendString("OK: GPIO mode\r\n");
            } else if (evt == EVT_STATUS) {
                UART_SendString("STATUS: PWM mode, running\r\n");
            } else {
                UART_SendString("ERROR: invalid event in PWM_DRIVE state\r\n");
            }
            break;

        case STATE_GPIO_DRIVE:
            if (evt == EVT_OFF) {
                current_state = STATE_STOP;
                Motor_Stop();
                UART_SendString("OK: motor stopped\r\n");
            } else if (evt == EVT_MODE_PWM) {
                current_state = STATE_PWM_DRIVE;
                Motor_SetMode(1);
                UART_SendString("OK: PWM mode\r\n");
            } else if (evt == EVT_DIR_FORWARD) {
                Motor_SetDirection(1);
                UART_SendString("OK: forward\r\n");
            } else if (evt == EVT_DIR_REVERSE) {
                Motor_SetDirection(2);
                UART_SendString("OK: reverse\r\n");
            } else if (evt == EVT_STATUS) {
                UART_SendString("STATUS: GPIO mode\r\n");
            } else {
                UART_SendString("ERROR: invalid event in GPIO_DRIVE state\r\n");
            }
            break;

        case STATE_FORWARD:
            if (evt == EVT_OFF) {
                current_state = STATE_STOP;
                Motor_Stop();
                UART_SendString("OK: motor stopped\r\n");
            } else if (evt == EVT_SPEED) {
                int pwm_val = param * 10;
                if (pwm_val > 1000) pwm_val = 1000;
                Motor_SetPWM(pwm_val);
                UART_SendString("OK: speed set\r\n");
            } else if (evt == EVT_DIR_REVERSE) {
                current_state = STATE_REVERSE;
                Motor_SetDirection(2);
                UART_SendString("OK: reverse\r\n");
            } else if (evt == EVT_MODE_GPIO) {
                current_state = STATE_GPIO_DRIVE;
                Motor_SetMode(0);
                UART_SendString("OK: GPIO mode\r\n");
            } else if (evt == EVT_MODE_PWM) {
                UART_SendString("OK: PWM mode (already)\r\n");
            } else if (evt == EVT_STATUS) {
                UART_SendString("STATUS: PWM mode, forward\r\n");
            } else {
                UART_SendString("ERROR: invalid event in FORWARD state\r\n");
            }
            break;

        case STATE_REVERSE:
            if (evt == EVT_OFF) {
                current_state = STATE_STOP;
                Motor_Stop();
                UART_SendString("OK: motor stopped\r\n");
            } else if (evt == EVT_SPEED) {
                int pwm_val = param * 10;
                if (pwm_val > 1000) pwm_val = 1000;
                Motor_SetPWM(pwm_val);
                UART_SendString("OK: speed set\r\n");
            } else if (evt == EVT_DIR_FORWARD) {
                current_state = STATE_FORWARD;
                Motor_SetDirection(1);
                UART_SendString("OK: forward\r\n");
            } else if (evt == EVT_MODE_GPIO) {
                current_state = STATE_GPIO_DRIVE;
                Motor_SetMode(0);
                UART_SendString("OK: GPIO mode\r\n");
            } else if (evt == EVT_MODE_PWM) {
                UART_SendString("OK: PWM mode (already)\r\n");
            } else if (evt == EVT_STATUS) {
                UART_SendString("STATUS: PWM mode, reverse\r\n");
            } else {
                UART_SendString("ERROR: invalid event in REVERSE state\r\n");
            }
            break;

        case STATE_ERROR:
            if (evt == EVT_ON) {
                current_state = STATE_STOP;
                Motor_Stop();
                UART_SendString("OK: recovered from error\r\n");
            } else {
                UART_SendString("ERROR: must send ON to recover\r\n");
            }
            break;

        default:
            break;
    }
}

void FSM_Process(void) {
    // 可添加周期性任务
}

void FSM_TriggerError(void) {
    FSM_DispatchEvent(EVT_ERROR, 0);
}

