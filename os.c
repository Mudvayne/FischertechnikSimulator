///os.c the task definition needed for FreeRTOS.

#include <stdio.h>
#include <stdint.h>

#include "../bricklib/utility/util_definitions.h"
#include "../bricklib/free_rtos/include/FreeRTOS.h"
#include "../bricklib/free_rtos/include/task.h"

#include "fischertechnik.c"
#include "program.h"

void wuberTechnik(void *parameters) {
	const uint32_t delay = 10; // Block for 10ms
	
	//Init Program
	initPins();
	initTimer();
	
	while(1) {		
		
		executeProgram();
		
		//Replace me with vTaskDelayUntil.
		TASK_DELAY_MS(delay);
	}
}
