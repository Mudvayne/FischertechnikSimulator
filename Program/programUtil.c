#include <stdbool.h>
#include <stdint.h>
#include "program.h"
#include "model.h"

// ##################################
// BEGIN SAFE_POSITION IMPLEMENTATION
// ##################################

bool firstPusherOnPosition;
bool secondPusherOnPosition;

void utilInitSafePosition() {
	firstPusherOnPosition = false;
	secondPusherOnPosition = false;
}

void utilComputeActionsForSafePosition() {
	firstPusherOnPosition = getFirstPusher()->isBackTriggerActivated;
	secondPusherOnPosition = getSecondPusher()->isBackTriggerActivated;
}

void stopWhenNeeded(bool onPosition, Pusher *pusher) {
	if(onPosition) {
		stopPusher(pusher);
	} else {
		runBackwardsPusher(pusher);
	}
}

void utilHandleActorsForSafePosition() {
	stopWhenNeeded(firstPusherOnPosition, getFirstPusher());
	stopWhenNeeded(secondPusherOnPosition, getSecondPusher());
}

bool isPusherDone(Pusher *pusher) {
	return pusher->isBackTriggerActivated && pusher->runningDirection == INACTIVE;
}

bool utilSafePositionDone() {
	bool firstPusherDone = isPusherDone(getFirstPusher());
	bool secondPusherDone = isPusherDone(getSecondPusher());

	return firstPusherDone && secondPusherDone;
}

// ##################################
// #END SAFE_POSITION IMPLEMENTATION#
// ##################################
// ###BEGIN DEPLETE IMPLEMENTATION###
// ##################################
#define STACK_SIZE 15
#define TIMEOUT_STAGE_SIX 3000

// BEGIN STACK
bool (*stack[STACK_SIZE])(void);
intptr_t currentStack;

void initStack() {
    currentStack = -1;
    for(intptr_t index = 0 ; index < STACK_SIZE; index++) {
        stack[index] = 0;
    }
}

void push(bool (*function)(void)) {
	currentStack++;
	if(currentStack >= STACK_SIZE) {
		//Exceptionhandling.
		printf("ERROR\n");
		return;
	}

	stack[currentStack] = function;
}

bool (*pop(void))(void) {
	bool (*function)(void) = stack[currentStack];
	stack[currentStack] = 0;

	currentStack--;
	if(currentStack < -1) {
		//Exceptionhandling.
		printf("ERROR\n");
		return 0;
	}

	return function;
}

bool (*peek(void))(void) {
	if(currentStack < 0 || currentStack >= STACK_SIZE) {
		return 0;
	}

	return stack[currentStack];
}

void pushUnderCurrent(bool (*function)(void)) {
    bool (*currentFunction)(void) = pop();
    push(function);
    push(currentFunction);
}
// END STACK

uint32_t deltaInMs;

bool depleteDone;

bool utilTurnOffAll() {
    stopTreadmill(getFirstTreadmill());
    stopTreadmill(getSecondTreadmill());
    stopTreadmill(getThirdTreadmill());
    stopTreadmill(getFourthTreadmill());

    stopPusher(getFirstPusher());
    stopPusher(getSecondPusher());
}

bool utilStageSix() {
	utilTurnOffAll();
	startTreadmill(getFourthTreadmill());

	return deltaInMs >= TIMEOUT_STAGE_SIX && !getFifthLightBarrier()->isBlocked;
}


enum PusherStageState{
    Start,
    Going_Forward,
    Going_Backward,
    Done
};

enum PusherStageState stageFiveState;
void initStageFive() {
    stageFiveState = Start;
}
bool utilStageFive() {
    utilTurnOffAll();
    Pusher *pusher = getSecondPusher();
    bool isDone = false;

    if(stageFiveState == Start) {
        runForwardPusher(pusher);
        stageFiveState = Going_Forward;

    } else if (stageFiveState == Going_Forward) {
        runForwardPusher(pusher);

        if(pusher->isFrontTriggerActivated) {
            runBackwardsPusher(pusher);
            stageFiveState = Going_Backward;
        }
    } else if (stageFiveState == Going_Backward) {
        runBackwardsPusher(pusher);

        if(pusher->isBackTriggerActivated) {
            stopPusher(pusher);

            isDone = true;
            pushUnderCurrent(&utilStageSix);
            stageFiveState = Done;
        }
    }

    return isDone;
}

void initStageFour() {

}
bool utilStageFour() {
    utilTurnOffAll();
    startTreadmill(getFirstTreadmill());
}

void utilInitDeplete() {
    initStack();

    deltaInMs = 0;
	depleteDone = false;

    push(&utilStageFour);
	push(&utilStageFive);
	push(&utilStageSix);

	initStageFive();
}

void utilComputeActionsForDeplete() {
    deltaInMs += totalSystem.timeDiffSinceLastCall;
    bool (*currentFunction)(void) = peek();

	if(currentFunction()) {
        deltaInMs = 0;
		pop();

		if(currentStack == -1) {
			depleteDone = true;
		}
	}
}

void utilHandleActorsForDeplete() {
	//Do nothing for now.
}

bool utilDepleteDone() {
	return depleteDone;
}

// ##################################
// ####END DEPLETE IMPLEMENTATION####
// ##################################
