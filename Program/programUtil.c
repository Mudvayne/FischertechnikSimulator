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
#define ITEM_TRACK_SIZE 4

#define TIMEOUT_STAGE_SIX_FULL 2000
#define TIMEOUT_STAGE_SIX 3000

#define HAND_OVER_STAGE_FOUR_BEGIN 2000
#define HAND_OVER_STAGE_FOUR_END 3000

#define TIMEOUT_STAGE_FOUR_END 8000

#define HAND_OVER_STAGE_THREE_BEGIN 1500
#define HAND_OVER_STAGE_THREE_END 3000

#define HAND_OVER_STAGE_ONE 800


// BEGIN STACK
bool (*stack[STACK_SIZE])(void);
intptr_t currentStack;

void initStack() {
    currentStack = -1;
    intptr_t index = 0;
    for(index ; index < STACK_SIZE; index++) {
        stack[index] = 0;
    }
}

void push(bool (*function)(void)) {
	currentStack++;
	if(currentStack >= STACK_SIZE) {
		//Exceptionhandling.
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

struct UtilStageSixData {
    uint16_t tmRunning;
    bool isRunning;
};

struct UtilStageSixData utilStageSixData;

void initStageSix() {
    utilStageSixData.tmRunning = 0;
    utilStageSixData.isRunning = true;
}

bool utilStageSix() {
	utilTurnOffAll();
	bool isDone = false;

	if(deltaInMs >= TIMEOUT_STAGE_SIX && !getFifthLightBarrier()->isBlocked) {
        isDone = true;
	} else if (deltaInMs >= TIMEOUT_STAGE_SIX && getFifthLightBarrier()->isBlocked) {

        utilStageSixData.tmRunning += totalSystem.timeDiffSinceLastCall;

        if(utilStageSixData.tmRunning >= TIMEOUT_STAGE_SIX_FULL) {
            utilStageSixData.isRunning = !utilStageSixData.isRunning;

            utilStageSixData.tmRunning = 0;
        }
	}

	if(utilStageSixData.isRunning) {
        startTreadmill(getFourthTreadmill());
	}

    return isDone;
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

bool utilHandlePusher(Pusher *pusher, enum PusherStageState *pusherStageState, bool (*nextFunction)(void), void (*initNextFunction)(void)) {
    bool isDone = false;

    if((*pusherStageState) == Start) {
        runForwardPusher(pusher);
        (*pusherStageState) = Going_Forward;

    } else if ((*pusherStageState) == Going_Forward) {
        runForwardPusher(pusher);

        if(pusher->isFrontTriggerActivated) {
            runBackwardsPusher(pusher);
            (*pusherStageState) = Going_Backward;
        }
    } else if ((*pusherStageState) == Going_Backward) {
        runBackwardsPusher(pusher);

        if(pusher->isBackTriggerActivated) {
            stopPusher(pusher);

            isDone = true;
            (*initNextFunction)();
            pushUnderCurrent(nextFunction);
            (*pusherStageState) = Done;
        }
    }

    return isDone;
}
bool utilStageFive() {
    utilTurnOffAll();
    return utilHandlePusher(getSecondPusher(), &stageFiveState, &utilStageSix, &initStageSix);
}

struct Item {
    bool exist;
    bool inLB;
    uint16_t position;
};
void utilInitItem(struct Item *item) {
    item->exist = false;
    item->inLB = false;
    item->position = 0;
}

struct ItemTrack {
    struct Item items[ITEM_TRACK_SIZE];
};
void utilInitItemTrack(struct ItemTrack *itemTrack) {
    utilInitItem(&(itemTrack->items[0]));
    utilInitItem(&(itemTrack->items[1]));
    utilInitItem(&(itemTrack->items[2]));
    utilInitItem(&(itemTrack->items[3]));
}

struct UtilStageToolData {
    uint16_t tmRunTime;
    bool lastLBTrigger;
    struct ItemTrack itemTrack;
};
struct UtilStageToolData utilStageFourData;

void utilMonitorItems(LightBarrier *lightBarrier, bool *lastLBBlocked, struct ItemTrack *itemTrack) {
    bool lbBlocked = lightBarrier->isBlocked;

    //Check for change.
    if(lbBlocked != (*lastLBBlocked)) {
        uint8_t index = 0;

        for(index = 0; index < ITEM_TRACK_SIZE; index++) {
            if (!itemTrack->items[index].exist) {
                break;
            }
        }

        if(lbBlocked && !(*lastLBBlocked)) { //New Item have been found.
            itemTrack->items[index].exist = true;
            itemTrack->items[index].inLB = true;

        } else if (!lbBlocked && (*lastLBBlocked)) { //Current Item left lb
            index--;

            itemTrack->items[index].inLB = false;
        }
    }

    //Update all items.
    for(uint8_t index = 0; index < ITEM_TRACK_SIZE; index++) {
        if (itemTrack->items[index].exist && !itemTrack->items[index].inLB) {
            itemTrack->items[index].position += totalSystem.timeDiffSinceLastCall;
        }
    }

    (*lastLBBlocked) = lbBlocked;
}
void initStageFour() {
    utilStageFourData.tmRunTime = 0;
    utilInitItemTrack(&utilStageFourData.itemTrack);

    if(getFourthLightBarrier()->isBlocked) {
        utilStageFourData.itemTrack.items[0].exist = true;
        utilStageFourData.itemTrack.items[0].inLB = true;
        utilStageFourData.lastLBTrigger = true;
    } else {
        utilStageFourData.lastLBTrigger = false;
    }
}

bool utilStageFourBegin() {
    bool isDone = false;

    utilTurnOffAll();
    utilMonitorItems(getFourthLightBarrier(), &utilStageFourData.lastLBTrigger, &utilStageFourData.itemTrack);

    if(deltaInMs >= HAND_OVER_STAGE_FOUR_BEGIN) {
        isDone = true;
        initStageFive();
        pushUnderCurrent(&utilStageFive);
    } else {
        startTreadmill(getThirdTreadmill());
    }

    return isDone;
}

enum NextAction {
    ACTION_NOT_DONE,
    ACTION_HAND_OVER,
    ACTION_TIME_OUT
};

enum NextAction handleTreadmill(uint16_t *tmRunning, struct ItemTrack *itemTrack, uint16_t handOverTime, uint16_t tmTimeout, Treadmill *treadmill) {
    enum NextAction action = ACTION_NOT_DONE;

    for(uint8_t index = 0; index < ITEM_TRACK_SIZE; index++) {
        if (itemTrack->items[index].exist) {

            if(itemTrack->items[index].position >= handOverTime) {
                action = ACTION_HAND_OVER;

                stopTreadmill(treadmill);
                itemTrack->items[index].exist = false;
            }
            break;
        }
    }

    if ((*tmRunning) >= tmTimeout) {
        action = ACTION_TIME_OUT;
    } else {

        startTreadmill(treadmill);
        (*tmRunning) += totalSystem.timeDiffSinceLastCall;
    }

    return action;
}

bool utilStageFourEnd() {
    bool isDone = false;
    enum NextAction action = ACTION_NOT_DONE;
    utilTurnOffAll();

    utilMonitorItems(getFourthLightBarrier(), &utilStageFourData.lastLBTrigger, &utilStageFourData.itemTrack);
    action = handleTreadmill(&utilStageFourData.tmRunTime, &utilStageFourData.itemTrack, HAND_OVER_STAGE_FOUR_END, TIMEOUT_STAGE_FOUR_END, getThirdTreadmill());

    if(action == ACTION_HAND_OVER) {
        isDone = true;
        initStageFive();
        pushUnderCurrent(&utilStageFourEnd);
        pushUnderCurrent(&utilStageFive);
    } else if (action == ACTION_TIME_OUT) {
        isDone = true;
        initStageFour();

    }

    return isDone;
}

struct UtilStageToolData utilStageThreeData;

void initStageThree() {
    utilStageThreeData.tmRunTime = 0;
    utilInitItemTrack(&utilStageThreeData.itemTrack);

    if(getThirdLightBarrier()->isBlocked) {
        utilStageThreeData.itemTrack.items[0].exist = true;
        utilStageThreeData.itemTrack.items[0].inLB = true;
        utilStageThreeData.lastLBTrigger = true;
    } else {
        utilStageThreeData.lastLBTrigger = false;
    }
}
bool utilStageThreeBegin() {
    bool isDone = false;

    utilTurnOffAll();
    utilMonitorItems(getThirdLightBarrier(), &utilStageThreeData.lastLBTrigger, &utilStageThreeData.itemTrack);

    if(deltaInMs >= HAND_OVER_STAGE_THREE_BEGIN) {
        isDone = true;
        pushUnderCurrent(&utilStageFourEnd);
    } else {
        startTreadmill(getSecondTreadmill());
    }

    return isDone;
}
bool utilStageThreeEnd() {
    bool isDone = false;
    enum NextAction action = ACTION_NOT_DONE;
    utilTurnOffAll();

    utilMonitorItems(getThirdLightBarrier(), &utilStageThreeData.lastLBTrigger, &utilStageThreeData.itemTrack);
    action = handleTreadmill(&utilStageThreeData.tmRunTime, &utilStageThreeData.itemTrack, HAND_OVER_STAGE_THREE_END, TIMEOUT_STAGE_FOUR_END, getSecondTreadmill());

    if(action == ACTION_HAND_OVER) {
        isDone = true;

        pushUnderCurrent(&utilStageThreeEnd);
        pushUnderCurrent(&utilStageFourEnd);
    } else if (action == ACTION_TIME_OUT) {
        isDone = true;
        initStageThree();

    }

    return isDone;
}

enum PusherStageState stageTwoState;

void initStageTwo() {
    stageTwoState = Start;
}

void doNothing() {

}

bool utilStageTwo() {
    utilTurnOffAll();
    return utilHandlePusher(getFirstPusher(), &stageTwoState, &utilStageThreeEnd, &doNothing);
}

struct StageOneData {
    bool lastLBBlocked;
    bool leftLB;
    uint16_t timer;
};
struct StageOneData stageOneState;

void initStageOne() {
    stageOneState.leftLB = false;
    stageOneState.timer = 0;
    if(getSecondLightBarrier()->isBlocked) {
        stageOneState.lastLBBlocked = true;
    } else {
        stageOneState.lastLBBlocked = true;
    }
}

bool utilStageOne() {
    utilTurnOffAll();
    bool isDone = false;

    if (getFirstLightBarrier()->isBlocked) {
        deltaInMs = 0;
    }

    bool lbBlocked = getSecondLightBarrier()->isBlocked;
    if (!lbBlocked && stageOneState.lastLBBlocked) {
        stageOneState.leftLB = true;
    }
    stageOneState.lastLBBlocked = lbBlocked;

    if(stageOneState.leftLB) {
        stageOneState.timer+= totalSystem.timeDiffSinceLastCall;

        if(stageOneState.timer >= HAND_OVER_STAGE_ONE) {
            stageOneState.timer = 0;
            stageOneState.leftLB = false;

            initStageTwo();
            pushUnderCurrent(&utilStageOne);
            pushUnderCurrent(&utilStageTwo);
            isDone = true;
        }
    }

    if(deltaInMs >= TIMEOUT_STAGE_FOUR_END) {
        isDone = true;
    }

    if(!isDone) {
        startTreadmill(getFirstTreadmill());
    }

    return isDone;
}

void utilInitDeplete() {
    initStack();

    deltaInMs = 0;
	depleteDone = false;

    push(&utilStageOne);
    push(&utilStageTwo);
    push(&utilStageThreeEnd);
    push(&utilStageThreeBegin);
    push(&utilStageFourEnd);
    push(&utilStageFourBegin);
	push(&utilStageFive);
	push(&utilStageSix);

    initStageSix();
	initStageFive();
	initStageFour();
	initStageThree();
	initStageTwo();
	initStageOne();
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
