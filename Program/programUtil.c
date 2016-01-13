#include <stdbool.h>
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

enum DepleteState {
	SIXTH_STAGE,
	FIFTH_STAGE,
	FOURTH_STAGE,
	THIRD_STAGE,
	SECOND_STAGE,
	FIRST_STAGE
};

enum DepleteState depleteState;

void utilInitDeplete() {
	depleteState = SIXTH_STAGE;
}

void utilComputeActionsForDeplete() {
	
}

void utilHandleActorsForDeplete() {
	
}

bool utilDepleteDone() {
	
}

// ##################################
// ####END DEPLETE IMPLEMENTATION####
// ##################################
