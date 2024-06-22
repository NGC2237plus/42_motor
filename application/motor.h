/**
 * @file motor.h
 * @author ‘Á…œªµ (star32349@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-06-22
 *
 * @copyright Copyright (c) 2024 ‘Á…œªµ
 *  All rights reserved.
 *
 * @see https://github.com/NGC2237plus/42_motor
 */
#ifndef __motor_H
#define __motor_H
#include "main.h"
#include "tim.h"
#include "gpio.h"

void Motor(uint8_t DIR, uint16_t Angle, uint16_t Angle_Speed);

#endif