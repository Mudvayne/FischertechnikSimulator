#include "program.h"
#include "model.h"
#include "treadmill.h"
#include "lightBarrier.h"

//"Automats" are here temporary, refactor into model.
int automatTmp = 0;
int automatTmp2 = 0;

void aggregateSensors() {
    LightBarrier *startLightBarrier = getFirstLightBarrier();
    LightBarrier *endLightBarrier = getSecondLightBarrier();

    automatTmp = startLightBarrier->isBlocked;
    automatTmp2 = endLightBarrier->isBlocked;
}

void computeActions() {
    //Tmp automat doesn't have a logic.
}

void handleAktors() {
    Treadmill *treadMill = getFirstTreadmill();

    if(automatTmp) {
        startTreadmill(treadMill);
    }
    if (automatTmp2) {
        stopTreadmill(treadMill);
    }
}

void executeProgram() {
    aggregateSensors();
    computeActions();
    handleAktors();
}
