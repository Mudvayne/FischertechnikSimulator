#include <stdbool.h>
#include <stdio.h>
#include "program.h"

enum InnerState {
	SAFE_POSITION,
	DEPLETE
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
	}		
}

void startHandleActors() {
	switch(state) {
		case SAFE_POSITION:
			utilHandleActorsForSafePosition();
			
			if(utilSafePositionDone()) {
				state = DEPLETE;
				printf("DEPLETE");
			}
	
			break;
	}
}
