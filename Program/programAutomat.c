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
        break;
    case DIAGNOSTIC:
        return &debugComputeActions;
        break;

    case PANIC_SWITCH:
    default:
        setSiteState(PANIC_SWITCH);
        return &panicComputeActions;
    }
}

void (*resolveHandleActorsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
        return &runningHandleActors;
        break;
    case DIAGNOSTIC:
        return &debugHandleActors;
        break;

    case PANIC_SWITCH:
    default:
        setSiteState(PANIC_SWITCH);
        return &panicHandleActors;
    }
}
