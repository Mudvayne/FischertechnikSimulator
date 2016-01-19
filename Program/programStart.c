#include <stdbool.h>
#include <stdio.h>
#include "program.h"
#include "model.h"

enum InnerState {
	SAFE_POSITION,
	DEPLETE,
	ASSURE_SAFE,
	DONE
};

enum InnerState state;

void startInit() {
	state = SAFE_POSITION;
	utilInitSafePosition();
}

void startComputeActions() {
	switch(state) {
        case ASSURE_SAFE:
		case SAFE_POSITION:
			utilComputeActionsForSafePosition();
			break;
        case DEPLETE:
            utilComputeActionsForDeplete();
            break;
	}
}

void startHandleActors() {
	switch(state) {
		case SAFE_POSITION:
			utilHandleActorsForSafePosition();

			if(utilSafePositionDone()) {
				state = DEPLETE;
				utilInitDeplete();
			}
			break;

        case DEPLETE:
            utilHandleActorsForDeplete();

            if(utilDepleteDone()) {
                state = ASSURE_SAFE;
                utilInitSafePosition();
            }
            break;

        case ASSURE_SAFE:
			utilHandleActorsForSafePosition();

			if(utilSafePositionDone()) {
				state = DONE;
				setSiteState(RUNNING, CODE_NO_ERROR);
			}
			break;
	}
}
