/**
 * @file motor.c
 * @author 早上坏 (star32349@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-07-03
 *
 * @copyright Copyright (c) 2024 成都大学(Chengdu University)
 *  All rights reserved.
 *
 * @see https://github.com/NGC2237plus/42_motor
 */
#include "motor.h"

#define Pulse_rev 800  // 细分32/800,Pulse/rev=800
#define Time_Cycle 100 // Timer cycle
__IO int Time_Count;   // count
int time0 = 0, time1 = 0;

// 4 motors, knee and hip joints of the right leg, and knee and hip joints of the left leg
Step_Motor Motor_Knee_R, Motor_Knee_L, Motor_Hip_R, Motor_Hip_L;

// timer interrupt
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        Time_Count++;
        if (Time_Count >= 1000000)
        {
            Time_Count = 0;
        }
        time0++;
        if (time0 >= 100000) // 1秒,1 second
        {
            time1++;
            time0 = 0;
        }
    }
}
/**
 * @brief Motor initialization
 *
 * @param motor Motor pointer
 * @param PUL_GPIOx Pin of motor PUL
 * @param PUL_Pin Pin of motor PUL
 * @param DIR_GPIOx Pin of motor DIR
 * @param DIR_Pin Pin of motor DIR
 */
void Motor_Init(Step_Motor *motor, GPIO_TypeDef *PUL_GPIOx, uint16_t PUL_Pin, GPIO_TypeDef *DIR_GPIOx, uint16_t DIR_Pin)
{
    motor->MOTOR_PUL_GPIOx = PUL_GPIOx;
    motor->MOTOR_PUL_Pin = PUL_Pin;
    motor->MOTOR_DIR_GPIOx = DIR_GPIOx;
    motor->MOTOR_DIR_Pin = DIR_Pin;
}
/**
 * @brief motor set
 *
 * @param DIR Turning, 0 is counterclockwise, 1 is clockwise
 * @param Angle The size of the angle to rotate, in degrees, from 0 to 360
 * @param Angle_Speed Rotation speed in degrees
 */
void Motor_Set(Step_Motor *Motor, uint8_t DIR, uint16_t Angle, uint16_t Angle_Speed)
{
    Motor->Frequency = (Angle_Speed / 360.0f) * Pulse_rev; // Frequency HZ
    Motor->Cycle = 1000000 / Motor->Frequency;             // Period us
    Motor->PUL_Number = (Angle / 360.0f) * Pulse_rev;      // Number of pulses
    Motor->PUL_Count = Motor->State = Motor->Last_State = 0;
    Motor->Angle = Angle;
    Motor->DIR = DIR;
    Motor->Angle_Speed = Angle_Speed;
    HAL_GPIO_WritePin(Motor->MOTOR_DIR_GPIOx, Motor->MOTOR_DIR_Pin, DIR);
}
/**
 * @brief Starting motor
 *
 */
void Motor_Start(void)
{
    uint8_t Exit = 0;
    Time_Count = 0;
    while (Exit < 4)
    {
        Exit = Motor_PWM(&Motor_Hip_R) + Motor_PWM(&Motor_Hip_L) + Motor_PWM(&Motor_Knee_R) + Motor_PWM(&Motor_Knee_L);
    }
}
/**
 * @brief Motor PWM wave drive
 *
 * @param Motor Motor pointer
 * @return uint8_t 0 indicates that the target position has not been reached yet, 1 indicates that the target position has been reached
 */
uint8_t Motor_PWM(Step_Motor *Motor)
{
    if (Motor->PUL_Count < Motor->PUL_Number && Motor->Angle_Speed > 0)
    {
        if ((Time_Count % (Motor->Cycle / Time_Cycle)) < (Motor->Cycle / Time_Cycle / 2))
        {
            HAL_GPIO_WritePin(Motor->MOTOR_PUL_GPIOx, Motor->MOTOR_PUL_Pin, 1);
            Motor->State = 0;
        }
        else
        {
            HAL_GPIO_WritePin(Motor->MOTOR_PUL_GPIOx, Motor->MOTOR_PUL_Pin, 0);
            Motor->State = 1;
        }

        if (Motor->Last_State == 1 && Motor->State == 0)
        {
            Motor->PUL_Count++;
        }
        Motor->Last_State = Motor->State;
        return 0;
    }
    else
    {
        return 1;
    }
}
