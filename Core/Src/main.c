#include "main.h"
#include "tim.h"
#include "motor.h"
#include "encoder.h"
#include "fsm.h"
#include "usart.h"
#include "pid.h"
#include <stdio.h>   // 添加 sprintf 支持


ControlMode control_mode = MODE_AUTO_PWM;
PID_HandleTypeDef pid;
uint8_t pid_enabled = 0;
float target_rpm = 0.0f;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();

    Encoder_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    PID_Init(&pid, 1.0f, 0.1f, 0.0f);

    uint32_t pwm_duty = 0;
    uint8_t dir = 1;
    uint32_t last_pwm_update = 0;
    uint32_t last_rpm_report = 0;

    while (1)
    {
        uint32_t now = HAL_GetTick();

        if (control_mode == MODE_AUTO_PWM) {
            if (now - last_pwm_update >= 30) {
                if (dir) {
                    pwm_duty += 5;
                    if (pwm_duty >= 1000) dir = 0;
                } else {
                    pwm_duty -= 5;
                    if (pwm_duty <= 0) dir = 1;
                }
                Motor_SetPWM(pwm_duty);
                last_pwm_update = now;
            }
        } else { // MODE_PID_CLOSED
            if (pid_enabled) {
                float real_rpm = Encoder_GetRPM();
                float pid_out = PID_Update(&pid, target_rpm, real_rpm);
                int16_t pwm_val = (int16_t)(pid_out);
                if (pwm_val > 1000) pwm_val = 1000;
                if (pwm_val < -1000) pwm_val = -1000;
                Motor_SetPWM(pwm_val);
            }
        }

        if (control_mode == MODE_PID_CLOSED && (now - last_rpm_report >= 1000)) {
            char buf[64];
            float real_rpm = Encoder_GetRPM();
            sprintf(buf, "Target: %.0f rpm -> Actual: %.0f rpm\r\n", target_rpm, real_rpm);
            UART_SendString(buf);
            last_rpm_report = now;
        }

        Encoder_UpdateSpeed();

        FSM_Process();

        HAL_Delay(10);
    }
}

/* 系统时钟配置（STM32F103） */
void SystemClock_Config(void)
{
    // ... 保持原有内容不变 ...
}

/* GPIO初始化（空实现，可自定义） */
static void MX_GPIO_Init(void)
{
    // ... 保持原有内容不变 ...
}

/* 错误处理 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // 断言失败处理（自定义）
}
#endif
