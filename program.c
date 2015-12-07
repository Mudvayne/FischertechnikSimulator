#include "program.h"
#include "model.h"
#include "treadmill.h"
#include "lightBarrier.h"

static int TRAVERSE_TIME_AFTER_FIRST_LB = 1100;
static int TRAVERSE_TIME_AFTER_CENTERED_LB = 500;

//"Automats" are here temporary, refactor into model.
int automatTmp = 0;
int automatTmp2 = 0;

void aggregateSensors() {
    LightBarrier *firstLightBarrier = getFirstLightBarrier();
    LightBarrier *secondLightBarrier = getSecondLightBarrier();
    LightBarrier *thridLightBarrier = getThirdLightBarrier();
    LightBarrier *fourthLightBarrier = getFourthLightBarrier();
    LightBarrier *fifthLightBarrier = getFifthLightBarrier();

    automatTmp = firstLightBarrier->isBlocked;
    automatTmp2 = secondLightBarrier->isBlocked;
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
