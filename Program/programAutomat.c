#include "programAutomat.h"

extern void runningInit();
extern void runningComputeActions();
extern void runningHandleActors();

extern void debugInit();
extern void debugComputeActions();
extern void debugHandleActors();

extern void panicInit();
extern void panicComputeActions();
extern void panicHandleActors();

extern void startInit();
extern void startComputeActions();
extern void startHandleActors();

SiteState lastState = PANIC_SWITCH;

void initNewState(SiteState currentState) {
	switch(currentState) {
    case RUNNING:
    case REST:
    case STOP:
        //todo: laststate is not set correctly so this is always false -> no init
        if(lastState != RUNNING && lastState != REST && lastState != STOP)
        {
            runningInit();
        }
        break;

    case DIAGNOSTIC:
        debugInit();
        break;

    case PANIC_SWITCH:
		panicInit();
		break;

	case START:
		startInit();
		break;
    }
}

void (*resolveComputeActionsFor(SiteState currentState))(void) {
	if(currentState != lastState) {
		lastState = currentState;
		initNewState(currentState);
	}

    switch(currentState) {
    case RUNNING:
    case REST:
    case STOP:
        return &runningComputeActions;

    case DIAGNOSTIC:
        return &debugComputeActions;

    case PANIC_SWITCH:
		return &panicComputeActions;

	case START:
		return &startComputeActions;

    default:
        setSiteState(PANIC_SWITCH, CODE_NOT_IMPLEMENTED);
        return &panicComputeActions;
    }
}

void (*resolveHandleActorsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
    case REST:
    case STOP:
        return &runningHandleActors;

    case DIAGNOSTIC:
        return &debugHandleActors;

    case PANIC_SWITCH:
		return &panicHandleActors;

	case START:
		return &startHandleActors;

    default:
        setSiteState(PANIC_SWITCH, CODE_NOT_IMPLEMENTED);
        return &panicHandleActors;
    }
}
