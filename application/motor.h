/**
 * @file motor.h
 * @author ���ϻ� (star32349@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-06-22
 *
 * @copyright Copyright (c) 2024 �ɶ���ѧ(Chengdu University)
 *  All rights reserved.
 *
 * @see https://github.com/NGC2237plus/42_motor
 */
#ifndef __motor_H
#define __motor_H
#include "main.h"
#include "tim.h"
#include "gpio.h"
typedef struct
{
    GPIO_TypeDef *MOTOR_PUL_GPIOx;
    uint16_t MOTOR_PUL_Pin;
    GPIO_TypeDef *MOTOR_DIR_GPIOx;
    uint16_t MOTOR_DIR_Pin;
    uint8_t DIR;          // ��ת����
    uint16_t Angle;       // ��ת�Ƕ�
    uint16_t Angle_Speed; // ��ת�ٶ�
    int Frequency;      // Ƶ��HZ
    int Cycle;          // ����us
    int PUL_Number;       // �������
    int PUL_Count;        // �������
    uint8_t State;
    uint8_t Last_State;
} Step_Motor;
extern Step_Motor Motor_Knee_R, Motor_Knee_L, Motor_Hip_R, Motor_Hip_L;

void Motor_Init(Step_Motor *motor, GPIO_TypeDef *PUL_GPIOx, uint16_t PUL_Pin, GPIO_TypeDef *DIR_GPIOx, uint16_t DIR_Pin);
void Motor_Set(Step_Motor *Motor, uint8_t DIR, uint16_t Angle, uint16_t Angle_Speed);
void Motor_Start(void);
uint8_t Motor_PWM(Step_Motor *Motor);


#endif
