#include "defines.h"
#include "model.h"
#include "programAutomat.h"
#include <stdlib.h>

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
    short timeout;
    short hasItemPassedSecondLB;
    short waitTime;
    short messureTimeBetweenItems;
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
    short isToolTime;
    short tMRuntimeLeftForCentering;
    short toolRuntimeLeft;
    short isTMRunning;
    short isToolRunning;
    short timeout;
    short updatedPosInToolTime;
} ToolStage;

typedef struct
{
    short itemCount;
    short isRunning;
    short lightBarrierBefore;
    short lightBarrierBlockedTime;
    short timeLeftForNextEmptyCheck;
    short tMRuntimeLeftForChecking;
    short isFull;
    short wasFullBefore;
    short isChecking;
    short timeout;
} StageSix;

typedef struct
{
    short LB1_2;
    short LB2_3;
    short LB3_4;
    short LB4_5;
    short LB1_2_count;
    short LB2_3_count;
    short LB3_4_count;
    short LB4_5_count;
} Timeouts;
#endif // __PROGRAMM

extern TotalSystem totalSystem;
extern StageOne stageOne;
extern PusherStage stageTwo;
extern ToolStage stageThree;
extern ToolStage stageFour;
extern PusherStage stageFive;
extern StageSix stageSix;
extern Timeouts timeouts;

extern void executeProgram();

