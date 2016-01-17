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
#define HAND_OVER_STAGE_FOUR_BEGIN 2000

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

    stopTool(getFirstTool());
    stopTool(getSecondTool());
}

bool utilStageSix() {
	utilTurnOffAll();

	if(deltaInMs >= TIMEOUT_STAGE_SIX && !getFifthLightBarrier()->isBlocked) {
        return true;
	} else {
        startTreadmill(getFourthTreadmill());
        return false;
	}
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

///struct  foobar

struct UtilStageFourData {
    uint8_t itemCount;
    bool lastLBTrigger;
};
struct UtilStageFourData utilStageFourData;

void initStageFour() {
    if(getFourthLightBarrier()->isBlocked) {
        utilStageFourData.itemCount = 1;
        utilStageFourData.lastLBTrigger = true;
    } else {
        utilStageFourData.itemCount = 0;
        utilStageFourData.lastLBTrigger = true;
    }
}
bool utilStageFourBegin() {
    bool isDone = false;
    utilTurnOffAll();

    bool lbBlocked = getFourthLightBarrier()->isBlocked;
    if(lbBlocked && !utilStageFourData.lastLBTrigger) {
        utilStageFourData.itemCount++;
    }
    utilStageFourData.lastLBTrigger = lbBlocked;

    if(deltaInMs >= HAND_OVER_STAGE_FOUR_BEGIN) {
        isDone = true;
        initStageFive();
        pushUnderCurrent(&utilStageFive);
    } else {
        startTreadmill(getThirdTreadmill());
    }

    return isDone;
}
bool utilStageFourEnd() {
    bool isDone = false;
    utilTurnOffAll();

}

void utilInitDeplete() {
    initStack();

    deltaInMs = 0;
	depleteDone = false;

    push(&utilStageFourBegin);
	push(&utilStageFive);
	push(&utilStageSix);

	initStageFive();
	initStageFour();
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
