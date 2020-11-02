//Liberias espressif
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
//Liberias Propias
#include "log_in.h"
#include "UARTconfig.c"
#include "WiFiconfig.h"
#include "webHTTP.h"
//Macros y Definiciones
#define PILA 1024
#define tamCOLA 1
#define tamMSN 1
//Variables globales
xQueueHandle Cola_UART;
extern uint8_t inic;
uint8_t parar = 0;

void app_main (void){
	nvs_flash_init();
	iniciarUART();
	configGPIO();
	Cola_UART = xQueueCreate(tamCOLA, tamMSN);
	xTaskCreatePinnedToCore(&log_in, "log_in", PILA*2, NULL, 5, NULL, 0);
	while(!parar){
		if (inic){
			iniciar_wifi();
			parar = 1;
		}
	}
	xTaskCreatePinnedToCore(&servidorhttp, "servidorHTTP", PILA*2, NULL, 5, NULL,1);
}
