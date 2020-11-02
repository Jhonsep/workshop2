#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include <string.h>
//Definiciones
#define tamBUFFER 1024
//Variables globales
extern xQueueHandle Cola_UART;
static QueueHandle_t uart0_queue;
//Función de eventos pra el UART0
static void TareaEventosUART(void *Parametro){
	uart_event_t evento;
	uint8_t* datoRX = (uint8_t*) malloc(tamBUFFER);
	for(;;){
		if (xQueueReceive(uart0_queue, (void*)&evento, (portTickType)portMAX_DELAY)){
			if (evento.type==UART_DATA){
				uart_read_bytes(UART_NUM_0, datoRX, evento.size, portMAX_DELAY);
				xQueueSend(Cola_UART, datoRX,0/portTICK_RATE_MS);
			}
		}
	}
	free(datoRX);
	datoRX=NULL;
	vTaskDelete(NULL);
}
//Función para iniciar el UART0
void iniciarUART(){
	uart_config_t configUART0 = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	};
	uart_param_config(UART_NUM_0, &configUART0);
	uart_set_pin(UART_NUM_0,(-1),(-1),(-1),(-1));
	uart_driver_install(UART_NUM_0, tamBUFFER*2, 0, 20, &uart0_queue, 0);
	xTaskCreatePinnedToCore(TareaEventosUART, "Tarea_para_uart_0", 1024*5, NULL, 6, NULL, 1);

}

