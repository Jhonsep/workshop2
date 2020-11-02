#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/api.h"
#include "driver/gpio.h"
#include <string.h>
#define PUERTO 80
#define LEDR 21
#define LEDG 22
#define LEDB 19
//variables globales
const char respuestaHTTP[] = "HTTP/1.1 200 OK\r\nContent-type:"
		"text/html\r\n\r\n";
const char abreHTML[] ="<html>";
const char cabeceraHTML[] =
		"<head>"
			"<meta charset =\"UTF-8\"/>"
			"<meta name=\"ledRGB\" content=\"Ejemplo para el control de LED RGB\"/>"
			"<meta name = \"keywords\" content =\"LED, RGB, ESP32, WiFi\"/>"
		"</head>";
const char cuerpoHTML[] =
		"<body style =\"background-color:#9AE0E7;\">"
			"<header>"
				"<h1 align=\"center\">"
					"<font color=\"navy\">"
						"control de iluminación"
					"</font>"
				"</h1>"
			"</header>"
			"<nav>"
				"<h2 aling = \"center\">"
					"<br>"
					"<p aling=\"center\">"
						"<a href=\"LEDR\">"
							"Cambiar Estado LED Rojo"
						"</a>"
					"</p>"
					"<br>"
					"<p aling=\"center\">"
						"<a href=\"LEDG\">"
							"Cambiar Estado LED Verde"
						"</a>"
					"</p>"
					"<br>"
					"<p aling=\"center\">"
						"<a href=\"LEDB\">"
							"Cambiar Estado LED Azul"
						"</a>"
					"</p>"
				"</h>"
			"</nav>"
		"</body>";
const char cierraHTML[] = "</html>";

void configGPIO(){
	gpio_set_direction(LEDR, GPIO_MODE_INPUT_OUTPUT);
	gpio_set_direction(LEDG, GPIO_MODE_INPUT_OUTPUT);
	gpio_set_direction(LEDB, GPIO_MODE_INPUT_OUTPUT);
}

static void WEBlocal(struct netconn *conexion){
	struct netbuf *bufferEntrada;
	char *buffer;
	u16_t long_buffer;
	err_t err;
	err = netconn_recv(conexion, &bufferEntrada);
	if (err == (ERR_OK)){
		printf("----- Paquete Recibido -----\n");
		netbuf_data(bufferEntrada, (void**)&buffer, &long_buffer);
		if (strncmp(buffer, "GET /",5) == 0){
			for (int i=0; buffer[i] != 0; i++) printf("%c", buffer[i]);
			printf("\n");
			if (strncmp(buffer, "GET /LEDR", 9) == 0){
				gpio_set_level(LEDR, !gpio_get_level(LEDR));
			}
			if (strncmp(buffer, "GET /LEDG", 9) == 0){
				gpio_set_level(LEDG, !gpio_get_level(LEDG));
			}
			if (strncmp(buffer, "GET /LEDB", 9) == 0){
				gpio_set_level(LEDB, !gpio_get_level(LEDB));
			}
			netconn_write(conexion, respuestaHTTP, sizeof(respuestaHTTP)-1,NETCONN_NOCOPY);
			netconn_write(conexion, abreHTML, sizeof(abreHTML)-1,NETCONN_NOCOPY);
			netconn_write(conexion, cabeceraHTML, sizeof(cabeceraHTML)-1,NETCONN_NOCOPY);
			netconn_write(conexion, cuerpoHTML, sizeof(cuerpoHTML)-1,NETCONN_NOCOPY);
			netconn_write(conexion, cierraHTML, sizeof(cierraHTML)-1,NETCONN_NOCOPY);
		}

	}
	netconn_close(conexion);
	netbuf_delete(bufferEntrada);
}

void servidorhttp(void *P){
	struct netconn *conectar, *NuevaCon;
		err_t err;
		conectar = netconn_new(NETCONN_TCP);
		netconn_bind(conectar, NULL, PUERTO);
		netconn_listen(conectar);
		do{
			err = netconn_accept(conectar, &NuevaCon);
			if (err == ERR_OK){
				WEBlocal(NuevaCon);
				netconn_delete(NuevaCon);
			}
		} while (err == (ERR_OK));
		netconn_close(conectar);
		netconn_delete(NuevaCon);
}
