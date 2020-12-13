/*
 * CO2Sensor.c
 *
 * Created: 2020/11/20 12:46:07
 *  Author: Taiquan Sui
 */

#include "co2_task.h"

#include "shared_data.h"

#include <stdio.h>

#include <mh_z19.h>


void co2_init() {
	mh_z19_create(ser_USART3, NULL);
	puts("[*] CO2: Initialized");
}

void co2_task(void* pvParams) {
	shared_data_t* sd = (shared_data_t*) pvParams;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();

	mh_z19_returnCode_t ret;
	uint16_t co2;
	while(1) {
		while((ret = mh_z19_takeMeassuring()) != MHZ19_OK) {
			printf("[!] CO2: Could not take measuring: %d\n", ret);
			vTaskDelay(50);
		}

		vTaskDelay(100);

		if((ret = mh_z19_getCo2Ppm(&co2)) != MHZ19_OK) {
			printf("[!] CO2: Could not get PPM: %d\n", ret);
			vTaskDelay(50);
			continue;
		}

		sd_setCo2(sd, co2);
		printf("[+] CO2: Measurement completed: %d\n", (int) sd_getCo2(sd));
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}
}