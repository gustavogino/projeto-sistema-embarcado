#ifndef PROJECT_HAL_H
#define PROJECT_HAL_H

//INIT FUNCTIONS
void START_HAL(void);
void CONFIG_CLOCK(void);
void START_GPIO(void);
void START_I2C(void);
void START_TIMER(void);
void START_UART(void);
void START_MAX30100(void);

//UTILS FUNTCTIONS
void MAX30100_Stop(void);
void MAX30100_Start(void);
void ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void UART_Receive(uint8_t *pData);
void UART_Transmit(uint8_t *pData, uint16_t timeout);
void I2C_Receive(uint16_t DevAddress, uint8_t *pData);
void I2C_Transmit(uint16_t DevAddress, uint8_t *pData);

#endif
