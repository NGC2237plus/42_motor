/**
 * @file motor.c
 * @author ���ϻ� (star32349@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-06-22
 *
 * @copyright Copyright (c) 2024 ���ϻ�
 *  All rights reserved.
 *
 * @see https://github.com/NGC2237plus/42_motor
 */
#include "motor.h"
// ��������
#define MOTOR_PUL_GPIOx GPIOA
#define MOTOR_PUL_Pin GPIO_PIN_3
#define MOTOR_DIR_GPIOx GPIOA
#define MOTOR_DIR_Pin GPIO_PIN_4

__IO int Time_Count;      // ����������ʱ������625us
int time0 = 0, time1 = 0; // ������

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        Time_Count++;
        time0++;
        if (time0 >= 1600) // 1��
        {
            time1++;
            time0 = 0;
        }
    }
}

/**
 * @brief �������
 *
 * @param DIR ת��0��ʱ�룬1˳ʱ��
 * @param Angle Ҫ��ת�ĽǶȴ�С����λ�ȣ�0~360
 * @param Angle_Speed ��ת���ٶȣ���λ�ȣ����ֵ720
 */
void Motor(uint8_t DIR, uint16_t Angle, uint16_t Angle_Speed)
{
    int Frequency = Angle_Speed / (360 * 0.00125f); // Ƶ��HZ
    int Cycle = 1000000 / Frequency;                // ����us
    int PUL_Number = (Angle / 360.f) * 800;         // �������

    HAL_GPIO_WritePin(MOTOR_DIR_GPIOx, MOTOR_DIR_Pin, DIR);
    int i;
    for (i = 0; i < PUL_Number && Angle_Speed > 0 && Angle_Speed <= 720; i++)
    {
        HAL_GPIO_WritePin(MOTOR_PUL_GPIOx, MOTOR_PUL_Pin, 1);
        Time_Count = 0;
        while (Time_Count < Cycle / 625 / 2) // �������
            ;
        HAL_GPIO_WritePin(MOTOR_PUL_GPIOx, MOTOR_PUL_Pin, 0);
        Time_Count = 0;
        while (Time_Count < Cycle / 625 / 2)
            ;
    }
}