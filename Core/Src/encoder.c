#include "main.h"
#include "encoder.h"
#include "tim.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_tim_ex.h"
#include "motor.h"
#include "fsm.h"

static int32_t encoder_total_count = 0;
static uint32_t last_count = 0;
static uint32_t last_time = 0;
static float current_rpm = 0.0f;
static uint16_t stall_counter = 0;
#define STALL_THRESHOLD 10
#define ENCODER_PPR 13

void Encoder_UpdateSpeed(void) {
    uint32_t now = HAL_GetTick();
    int32_t count = Encoder_GetCount();
    int32_t delta = count - last_count;
    uint32_t dt = now - last_time;
    if (dt >= 50) {
        current_rpm = (float)delta * 60000.0f / (ENCODER_PPR * dt);
        last_count = count;
        last_time = now;
    }
    int16_t pwm = Motor_GetPWM();
    float rpm = current_rpm;
    if (pwm != 0 && rpm < 5) {
        stall_counter++;
        if (stall_counter >= STALL_THRESHOLD) {
            FSM_TriggerError();
            stall_counter = 0;
        }
    } else {
        stall_counter = 0;
    }
}

float Encoder_GetRPM(void) {
    return current_rpm;
}

void Encoder_Init(void) {
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
}

void Encoder_UpdateIRQHandler(void) {
    if(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE) != RESET) {
        __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
        if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == TIM_COUNTERMODE_DOWN) {
            encoder_total_count -= 65536;
        } else {
            encoder_total_count += 65536;
        }
    }
}

int32_t Encoder_GetCount(void) {
    uint32_t count = __HAL_TIM_GET_COUNTER(&htim3);
    return encoder_total_count + (int32_t)count;
}

void Encoder_ResetCount(void) {
    encoder_total_count = 0;
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}
