#include "programAutomat.h"

extern void runningComputeActions();
extern void runningHandleAktros();

void (*resolveComputeActionsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
        return &runningComputeActions;
        break;
    }
}

void (*resolveHandleAktorsFor(SiteState currentState))(void) {
    switch(currentState) {
    case RUNNING:
        return &runningHandleAktros;
        break;
    }
}
