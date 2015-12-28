#include "defines.h"
#include "model.h"
#include "programAutomat.h"

//******************************************
//****** VARIABLES FOR STATE MACHINE *******
//******************************************
#ifndef __PROGRAMM
#define __PROGRAMM
typedef struct
{
    short itemsInSystem;
    long long timeDiffSinceLastCall;
} TotalSystem;

typedef struct
{
    short itemCount;
    int itemPositions[3];
    short firstLightBarrierBefore;
    short secondLightBarrierBefore;

    short isRunning;
    short isReady;
    short timeout;
    short hasItemPassedSecondLB;
    short waitTime;
} StageOne;

typedef struct
{
    short pusherDir;
    short isOccupied;
    short isReady;
} PusherStage;

typedef struct
{
    short itemCount;
    short itemCountBefore;
    int itemPositions[3];
    short isReady;
    short lightBarrierBefore;
		short hasItemPassedLightBarrier;
    short isToolTime; //centering and tooling
    short tMRuntimeLeftForCentering;
    short toolRuntimeLeft;
    short isTMRunning;
    short isToolRunning;
    short timeout;
    short isWaiting;
} ToolStage;

typedef struct
{
    short isReady;
    short itemCount;
    short isRunning;
    short lightBarrierBefore;
    short lightBarrierBlockedTime;
    short timeLeftForNextEmptyCheck;
    short tMRuntimeLeftForChecking;
    short isFull;
    short wasFullBefore;
    short timeout;
} StageSix;
#endif // __PROGRAMM

extern TotalSystem totalSystem;
extern StageOne stageOne;
extern PusherStage stageTwo;
extern ToolStage stageThree;
extern ToolStage stageFour;
extern PusherStage stageFive;
extern StageSix stageSix;

extern void executeProgram();

