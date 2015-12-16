#include <stdio.h>
#include <stdint.h>

#include "../bricklib/drivers/pio/pio.h"

#include "../bricklib/drivers/usart/uart_console.h"
#include "../bricklib/utility/util_definitions.h"

#include "../bricklib/free_rtos/include/FreeRTOS.h"
#include "../bricklib/free_rtos/include/task.h"

#include "lightBarrier.h"
#include "treadmill.h"
#include "pusher.h"
#include "tool.h"

#include "timeUtil.h"

/// See Fischertechnik_HW.pdf (in moodle) for explanation.
Pin pins[] = {
	{1 << 10, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q1,	0
	{1 << 29, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q2,	1
	{1 << 13, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q3,	2
	{1 << 2, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q4,	3
	{1 << 6, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q5,	4
	
	{1 << 11, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q6,	5
	{1 << 30, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q7,	6
	{1 << 14, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q8,	7
	{1 << 3, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q9,	8
	{1 << 7, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}, 	//Q10,	9
	
	{1 << 12, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I1,	10
	{1 << 25, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT}, 		//I2,	11
	{1 << 0, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I3,	12
	{1 << 4, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I4,	13
	
	{1 << 9, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I5,	14
	{1 << 13, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I6,	15
	{1 << 26, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT}, 		//I7,	16
	{1 << 1, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}, 		//I8,	17
	{1 << 5, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT} 		//I9,	18
};

// Arrays to determine pin-index.
const uint32_t treadmillsIndex[] = { 4, 5, 7, 9 };
const uint32_t lightBarrierIndex[] = { 16, 14, 15, 17, 18 };
const uint32_t pusherBackwardsIndex[] = { 0, 2 };
const uint32_t pusherForwardsIndex[] = { 1, 3 };
const uint32_t pusherUpperSensorIndex[] = { 10, 12 };
const uint32_t pusherLowerSensorIndex[] = { 11, 13 };
const uint32_t toolIndex[] = { 6, 8 };

///Inits the pins array.
void initPins() {
	uint32_t size = sizeof(pins) / sizeof(Pin);
	PIO_Configure(pins, size);
}

// ##################################
// BEGIN LIGHT_BARRIER IMPLEMENTATION
// ##################################

void setBlocked(LightBarrier *lightBarrier) {
	lightBarrier->isBlocked = 1;
}

void setUnblocked(LightBarrier *lightBarrier) {
	lightBarrier->isBlocked = 0;
}

void updateLightBarrier(LightBarrier *lightBarrier) {
	Pin *pin = &pins[lightBarrierIndex[lightBarrier->id]]
	
	if(PIO_Get(pin)) {
		setBlocked(lightBarrier);
	} else {
		setUnblocked(lightBarrier);
	}
}

// ##################################
// END LIGHT_BARRIER IMPLEMENTATION
// BEGIN PUSHER IMPLEMENTATION
// ##################################

void runForwardPusher(Pusher *pusher) {
	Pin *forwardPin = &pins[pusherForwardsIndex[pusher->id]];
	Pin *backwardPin = &pins[pusherBackwardsIndex[pusher->id]];
	
	//Make sure Backward-Pin is OFF.
	PIO_Set(backwardPin);
	//Then Go Forward.
	PIO_Clear(forwardPin);
	
	pusher->runningDirection = FORWARDS;
}

void runBackwardsPusher(Pusher *pusher) {
	Pin *forwardPin = &pins[pusherForwardsIndex[pusher->id]];
	Pin *backwardPin = &pins[pusherBackwardsIndex[pusher->id]];
	
	//Make sure Forward-Pin is OFF.
	PIO_Set(forward);
	//Then Go Backward.
	PIO_Clear(backward);
	
	pusher->runningDirection = BACKWARDS;
}

void stopPusher(Pusher *pusher) {
	Pin *forwardPin = &pins[pusherForwardsIndex[pusher->id]];
	Pin *backwardPin = &pins[pusherBackwardsIndex[pusher->id]];
	
	//Deactivate all pins
	PIO_Set(backwardPin);
	PIO_Set(forward);
	
	pusher->runningDirection = INACTIVE;
}
 
void updatePusher(Pusher *pusher) {
	Pin *upperSensor = &pins[pusherUpperSensorIndex[pusher->id]];
	Pin *lowerSensor = &pins[pusherLowerSensorIndex[pusher->id]];
	
	uint8_t current = PIO_Get(pin);

	//Check for change.
	if(PIO_Get(upperSensor)) {
		pusher->isFrontTriggerActivated = 1;
	} else {
		pusher->isFrontTriggerActivated = 0;
	}
	
	if(PIO_Get(lowerSensor)) {
		pusher->isBackTriggerActivated = 1;
	} else {
		pusher->isBackTriggerActivated = 0;
	}
}

// ##################################
// END PUSHER IMPLEMENTATION
// BEGIN TREADMILL IMPLEMENTATION
// ##################################

void startTreadmill(Treadmill *treadmill) {
	Pin *pin = &pins[treadmillsIndex[treadmill->id]];
	
	//Activate treadmill.
	PIO_Clear(pin);
}

void stopTreadmill(Treadmill *treadmill) {
	Pin *pin = &pins[treadmillsIndex[treadmill->id]];
	
	//Deactivate threadmill.
	PIO_Set(pin);
}

// ##################################
// END TREADMILL IMPLEMENTATION
// BEGIN TOOL IMPLEMENTATION
// ##################################

void startTool(Tool *tool) {
	Pin *pin = &pins[toolIndex[treadmill->id]];
	
	//Activate Tool.
	PIO_Clear(pin);
}

void stopTool(Tool *tool) {
	Pin *pin = &pins[toolIndex[treadmill->id]];
	
	//Deactivate Tool.
	PIO_Set(pin);
}

// ##################################
// END TOOL IMPLEMENTATION
// BEGIN TIME_UTIL
// ##################################

unsigned long lastTicks = 0;

void initTimer() {
	lastTicks = xTaskGetTickCount();
}

long long calculateTimeDiffSinceLastCall() {
	unsigned long currentTicks = xTasksGetTickCount();
	long long difference = currentTicks - lastTicks;
	lastTicks = currentTicks;
	
	return difference;
}

// ##################################
// END TIME_UTIL
// ##################################
