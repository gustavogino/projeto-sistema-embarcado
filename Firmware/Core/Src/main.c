#include "main.h"
#include "project_hal.h"
#include "functions.h"

#define MAX30100_I2C_ADDRESS 	0xAE
#define TIMEOUT 100

uint8_t RX_BLE_BUFFER[2] = {0};
uint8_t SENSOR_DATA_BUFFER[100] = {0};

STATUS CAPTURE = OFF;
GPIO_PinState LED_STATE = GPIO_PIN_RESET;

int main(void)
{	
	START_HAL();
	CONFIG_CLOCK();
	START_GPIO();
	START_I2C();
	START_TIMER();
	START_UART();
	START_MAX30100();
	
	UART_Receive(RX_BLE_BUFFER);
	
  while (1)
	{
			if			(RX_BLE_BUFFER[0] == '0')	CAPTURE = OFF;  	
    	else if	(RX_BLE_BUFFER[0] == '1')	CAPTURE = ON;
		
			switch(CAPTURE)
			{
				case OFF:
					WritePin(LED_PIN_GPIO_Port, LED_PIN, LED_STATE);					
					break;
				
				case ON:					
					UART_Transmit(SENSOR_DATA_BUFFER, TIMEOUT);
					break;					
			}
			Sleep_ms(100);
  }
}

void Interruption_TIM2(void)
{	
	LED_STATE = (LED_STATE == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

void Interruption_UART(void)
{
	UART_Receive(RX_BLE_BUFFER);	
	RX_BLE_BUFFER[0] == '1' ? MAX30100_Start() : MAX30100_Stop(); 	
	UART_Transmit(RX_BLE_BUFFER, TIMEOUT);	
}

void Interruption_I2C(void)
{
	I2C_Receive(MAX30100_I2C_ADDRESS, SENSOR_DATA_BUFFER);
}
