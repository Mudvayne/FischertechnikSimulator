#include "program.h"
#include "treadmill.h"
#include "lightBarrier.h"
#include "timeUtil.h"
#include "defines.h"

TotalSystem totalSystem =   {.itemsInSystem = 0, .timeDiffSinceLastCall = 0};
StageOne stageOne =         {.isRunning = 0, .firstLightBarrierBefore = 0, .isReady = 1, .itemCount = 0, .itemPositions = {-1, -1, -1}, .timeout = 0,
                             .hasItemPassedSecondLB = 0};
PusherStage stageTwo =      {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
ToolStage stageThree =      {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                             .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                             .isTMRunning = 0, .isToolRunning = 0, .timeout = 0, .hasItemPassedLightBarrier = 0, .isWaiting = 0};
ToolStage stageFour =       {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                             .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                             .isTMRunning = 0, .isToolRunning = 0, .timeout = 0, .hasItemPassedLightBarrier = 0, .isWaiting = 0};
PusherStage stageFive =     {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
StageSix stageSix =         {.isReady = 1, .itemCount = 0, .isRunning = 0, .lightBarrierBefore = 0, .lightBarrierBlockedTime = 0,
                             .timeLeftForNextEmptyCheck = -1, .tMRuntimeLeftForChecking = 0, .isFull = 0, .wasFullBefore = 0, .timeout = 0};

void aggregateSensors() {
    updatePusher(getFirstPusher());
    updatePusher(getSecondPusher());
    updateLightBarrier(getFirstLightBarrier());
    updateLightBarrier(getSecondLightBarrier());
    updateLightBarrier(getThirdLightBarrier());
    updateLightBarrier(getFourthLightBarrier());
    updateLightBarrier(getFifthLightBarrier());
}

void handleAktors() {
}

void debug()
{
    printf("ITEMS IN SYSTEM: %d, TOs MAX %ds: 1 = %d, 3 = %d, 4 = %d, 6 = %d          ", totalSystem.itemsInSystem, TIMEOUT/1000, stageOne.timeout, stageThree.timeout, stageFour.timeout, stageSix.timeout);
    printf("\nSTAGE 1: items = %d, pos = %d %d %d, running = %d, ready = %d \t", stageOne.itemCount, stageOne.itemPositions[0], stageOne.itemPositions[1], stageOne.itemPositions[2], stageOne.isRunning, stageOne.isReady);
    printf("\nSTAGE 2: occupied = %d, dir = %d,  ready = %d\t\t", stageTwo.isOccupied, stageTwo.pusherDir, stageTwo.isReady);
    printf("\nSTAGE 3: items = %d, pos = %d %d %d, TM = %d, toolLeft = %d, ready = %d    ", stageThree.itemCount, stageThree.itemPositions[0], stageThree.itemPositions[1], stageThree.itemPositions[2], stageThree.isTMRunning, stageThree.toolRuntimeLeft, stageThree.isReady);
    printf("\nSTAGE 4: items = %d, pos = %d %d %d, TM = %d, toolLeft = %d, ready = %d    ", stageFour.itemCount, stageFour.itemPositions[0], stageFour.itemPositions[1], stageFour.itemPositions[2], stageFour.isTMRunning, stageFour.toolRuntimeLeft, stageFour.isReady);
    printf("\nSTAGE 5: occupied = %d, dir = %d, ready = %d\t\t", stageFive.isOccupied, stageFive.pusherDir, stageFive.isReady);
    printf("\nSTAGE 6: items = %d, running = %d, lb blocked for = %d, next check in = %d   ", stageSix.itemCount, stageSix.isRunning, stageSix.lightBarrierBlockedTime, stageSix.timeLeftForNextEmptyCheck);
}


void executeProgram() {
    void (*computeActions)(void) = resolveComputeActionsFor(getSiteState());
    void (*handleActors) (void) = resolveHandleActorsFor(getSiteState());

    totalSystem.timeDiffSinceLastCall = calculateTimeDiffSinceLastCall();

    aggregateSensors();
    (*computeActions)();
    (*handleActors)();

    #ifndef ON_TARGET
    debug();
    #endif
}
