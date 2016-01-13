#include "programAutomat.h"

extern void runningComputeActions();
extern void runningHandleActors();

extern void debugComputeActions();
extern void debugHandleActors();

extern void panicComputeActions();
extern void panicHandleActors();

void (*resolveComputeActionsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
        return &runningComputeActions;

    case DIAGNOSTIC:
        return &debugComputeActions;

    case PANIC_SWITCH:
				return &panicComputeActions;

    default:
        setSiteState(PANIC_SWITCH, 200);
        return &panicComputeActions;
    }
}

void (*resolveHandleActorsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
        return &runningHandleActors;
    case DIAGNOSTIC:
        return &debugHandleActors;

    case PANIC_SWITCH:
				return &panicComputeActions;

    default:
        setSiteState(PANIC_SWITCH, 200);
        return &panicHandleActors;
    }
}
