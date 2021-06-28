#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"

#define I2C_SCL_SENSOR_Pin 					GPIO_PIN_9
#define I2C_SDA_SENSOR_Pin 					GPIO_PIN_10
#define LED_PIN											GPIO_PIN_15

#define I2C_SDA_SENSOR_GPIO_Port 		GPIOA
#define I2C_SCL_SENSOR_GPIO_Port 		GPIOA
#define LED_PIN_GPIO_Port 					GPIOA

typedef enum
{
  OFF = 0,
  ON
} STATUS;

void Interruption_TIM2(void);
void Interruption_UART(void);
void Interruption_I2C(void);

#ifdef __cplusplus
}
#endif

#endif
