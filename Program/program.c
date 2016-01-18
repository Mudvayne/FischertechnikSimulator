#include "program.h"
#include "treadmill.h"
#include "lightBarrier.h"
#include "timeUtil.h"
#include "defines.h"

TotalSystem totalSystem =   {.itemsInSystem = 0, .timeDiffSinceLastCall = 0};
StageOne stageOne =         {.isRunning = 0, .firstLightBarrierBefore = 0, .itemCount = 0, .itemPositions = {-1, -1, -1}, .timeout = 0, .hasItemPassedSecondLB = 0};
PusherStage stageTwo =      {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
ToolStage stageThree =      {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                             .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                             .isTMRunning = 0, .isToolRunning = 0, .timeout = 0, .hasItemPassedLightBarrier = 0, .updatedPosInToolTime = 0};
ToolStage stageFour =       {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                             .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                             .isTMRunning = 0, .isToolRunning = 0, .timeout = 0, .hasItemPassedLightBarrier = 0, .updatedPosInToolTime = 0};
PusherStage stageFive =     {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
StageSix stageSix =         {.itemCount = 0, .isRunning = 0, .lightBarrierBefore = 0, .lightBarrierBlockedTime = 0,
                             .timeLeftForNextEmptyCheck = -1, .tMRuntimeLeftForChecking = 0, .isFull = 0, .wasFullBefore = 0, .timeout = 0, .isChecking = 0};

void aggregateSensors() {
    updatePusher(getFirstPusher());
    updatePusher(getSecondPusher());
    updateLightBarrier(getFirstLightBarrier());
    updateLightBarrier(getSecondLightBarrier());
    updateLightBarrier(getThirdLightBarrier());
    updateLightBarrier(getFourthLightBarrier());
    updateLightBarrier(getFifthLightBarrier());
}

void debug()
{
    /*system("cls");
    printf("ITEMS IN SYSTEM: %d, TOs MAX %ds: 1 = %d, 3 = %d, 4 = %d, 6 = %d", totalSystem.itemsInSystem, TIMEOUT/1000, stageOne.timeout, stageThree.timeout, stageFour.timeout, stageSix.timeout);
    printf("\nSTAGE 1: items = %d, pos = %d %d %d, running = %d", stageOne.itemCount, stageOne.itemPositions[0], stageOne.itemPositions[1], stageOne.itemPositions[2], stageOne.isRunning);
    printf("\nSTAGE 2: occupied = %d, dir = %d,  ready = %d\t\t", stageTwo.isOccupied, stageTwo.pusherDir, stageTwo.isReady);
    printf("\nSTAGE 3: items = %d, pos = %d %d %d, TT = %d, toolLeft = %d, ready = %d", stageThree.itemCount, stageThree.itemPositions[0], stageThree.itemPositions[1], stageThree.itemPositions[2], stageThree.isToolTime, stageThree.toolRuntimeLeft, stageThree.isReady);
    printf("\nSTAGE 4: items = %d, pos = %d %d %d, TT = %d, toolLeft = %d, ready = %d", stageFour.itemCount, stageFour.itemPositions[0], stageFour.itemPositions[1], stageFour.itemPositions[2], stageFour.isToolTime, stageFour.toolRuntimeLeft, stageFour.isReady);
    printf("\nSTAGE 5: occupied = %d, dir = %d, ready = %d", stageFive.isOccupied, stageFive.pusherDir, stageFive.isReady);
    printf("\nSTAGE 6: items = %d, full = %d, lb blocked for = %d, next check in = %d", stageSix.itemCount, stageSix.isFull, stageSix.lightBarrierBlockedTime, stageSix.timeLeftForNextEmptyCheck);*/
}

void handlePanicSwitch() {
	if(getSiteState() == PANIC_SWITCH || getSiteState() == DIAGNOSTIC) {
		return;
	}

	uint8_t firstFrontTrigger = getFirstPusher()->isFrontTriggerActivated;
	uint8_t firstBackTrigger = getFirstPusher()->isBackTriggerActivated;
	uint8_t secondFrontTrigger = getSecondPusher()->isFrontTriggerActivated;
	uint8_t secondBackTrigger = getSecondPusher()->isBackTriggerActivated;

	if(firstFrontTrigger && firstBackTrigger && secondFrontTrigger && secondBackTrigger) {
		setSiteState(PANIC_SWITCH, CODE_PANIC_SWITCH);
	}
}

void executeProgram() {
    totalSystem.timeDiffSinceLastCall = calculateTimeDiffSinceLastCall();

    aggregateSensors();
    handlePanicSwitch();

    void (*computeActions)(void) = resolveComputeActionsFor(getSiteState());
    void (*handleActors) (void) = resolveHandleActorsFor(getSiteState());
    (*computeActions)();
    (*handleActors)();

    #ifndef ON_TARGET
    debug();
    #endif
}
