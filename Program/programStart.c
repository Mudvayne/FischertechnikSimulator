#include <stdbool.h>
#include <stdio.h>
#include "program.h"

enum InnerState {
	SAFE_POSITION,
	DEPLETE,
	FOOBAR
};

enum InnerState state;

void startInit() {
	state = SAFE_POSITION;
	utilInitSafePosition();
}

void startComputeActions() {
	switch(state) {
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
				printf("DEPLETE");
			}

			break;

        case DEPLETE:
            utilHandleActorsForDeplete();

            if(utilDepleteDone()) {
                state = FOOBAR;
                printf("FOOOOBAR");
            }
	}
}
