#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include "WiFiconfig.h"
#define ok 0x0D
extern xQueueHandle Cola_UART;
uint8_t inic = 0;

void log_in(void *P){
	char Rx[1];
	uint8_t ic = 0, completo = 0;
	uint8_t userok=0, passok=0;
	const char user[5]={"userx"}, pass[5]={"clave"};
	char almacen[6];
	for(;;){
		if(xQueueReceive(Cola_UART,&Rx,1000/portTICK_RATE_MS)==pdTRUE){
			uart_write_bytes(UART_NUM_0, (char*)Rx,1);
			almacen[ic]=Rx[0];
			if(almacen[ic] == ok){
				completo = 1;
				uart_write_bytes(UART_NUM_0, (char*)almacen,strlen(almacen));
				uart_write_bytes(UART_NUM_0, (char*)"\n=== Recibido ===\n\r",19);
			}
			ic++;
		}
		if (completo){
			ic = 0;
			completo = 0;
			if (strncmp(almacen,user,5)==0){
				userok=1;
			}
			if (strncmp(almacen,pass,5)==0){
				passok=1;
			}
			for (int i=0;i < strlen(almacen);i++)almacen[i]=0;
		}
		if (userok && passok){
			uart_write_bytes(UART_NUM_0, (char*)"\n--- Verificado ---\n\r",21);
			userok =0; passok=0;
			inic = 1;
			vTaskDelete(NULL);
		}
	}
}
