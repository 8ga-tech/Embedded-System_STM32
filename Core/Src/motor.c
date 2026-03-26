#include "motor.h"
#include "tim.h"
#include "gpio.h"

static uint8_t motor_mode = 1;   // 默认 PWM 模式
static uint8_t motor_dir = 0;
static int16_t current_pwm = 0;

void Motor_SetPWM(int16_t pwm) {
   current_pwm = pwm;
    if (pwm > 1000) pwm = 1000;
    if (pwm < -1000) pwm = -1000;
    if (motor_mode == 1) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (pwm > 0 ? pwm : -pwm));
    }
    
}
int16_t Motor_GetPWM(void) { return current_pwm; }
void Motor_Stop(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);
}

void Motor_SetDirection(uint8_t dir) {
    motor_dir = dir;
    if (motor_mode == 0) {
        // GPIO 模式：直接控制 IN1/IN2
        if (dir == 1) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        } else if (dir == 2) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);
        }
    } else {
        // PWM 模式：方向引脚仍用 GPIO 控制（假设 IN1/IN2 同时作为方向和使能）
        if (dir == 1) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        } else if (dir == 2) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);
        }
    }
}

void Motor_SetMode(uint8_t mode) {
    motor_mode = mode;
    if (mode == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0); // 停止 PWM
    }
}
