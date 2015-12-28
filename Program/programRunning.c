#include "program.h"

void timeout(int stage)
{
	#ifndef ON_TARGET
    system("cls");
	#endif

  printf("Stage %d expected an item which never arrived.\n", stage);

	stopTreadmill(getFirstTreadmill());
	stopTreadmill(getSecondTreadmill());
	stopTreadmill(getThirdTreadmill());
	stopTreadmill(getFourthTreadmill());

	stopTool(getFirstTool());
	stopTool(getSecondTool());

	#ifndef ON_TARGET
    getchar();
	#endif
}

void computeFirstTreadmill()
{
    // check if timeout
    if(getSecondLightBarrier()->isBlocked)
    {
        stageOne.timeout = 0;
        stageThree.timeout = 1;
    }
    if(stageOne.timeout > 0)
    {
        stageOne.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageOne.timeout >= TIMEOUT)
    {
        timeout(1);
    }
    //new items in stage?
    if(getFirstLightBarrier()->isBlocked == 1)
    {
        if(stageOne.firstLightBarrierBefore == 0)
        {
            stageOne.firstLightBarrierBefore = 1;
            // new item in LB 1
            totalSystem.itemsInSystem++;
            stageOne.itemCount++;
            int i = 0;
            for( i ; i < 3; i++)
            {
                if(stageOne.itemPositions[i] == -1)
                {
                    stageOne.itemPositions[i] = 0;
                    break;
                }
            }
        }
    }
    else if(stageOne.firstLightBarrierBefore == 1)
    {
        //item left first light barrier
        stageOne.firstLightBarrierBefore = 0;
        stageOne.timeout += 1;
    }

    //is this stage ready for more items?
    int i = 0;
    if(stageOne.itemCount > 0)
    {
        for( i ; i < 3 ; i++)
        {
            int pos = stageOne.itemPositions[i];
            if(pos >= 0 && pos <= 0 + EMPTY_SPACE_TO_BE_READY)
            {
                stageOne.isReady = 0;
                break;
            }
            else
            {
                stageOne.isReady = 1;
            }
        }
    }
    else
    {
        stageOne.isReady = 1;
    }

    //should it run?
    stageOne.isRunning = 0;
    if(stageOne.itemCount > 0)
    {
        stageOne.isRunning = 1;
        i = 0;
        for( i ; i < 3 ; i++)
        {
            if(getSecondLightBarrier()->isBlocked && stageTwo.isReady == 0)
            {
                stageOne.isRunning = 0;
                break;
            }
        }
    }

    //has a item passed the LB2?
    if(getSecondLightBarrier()->isBlocked == 0)
    {
        if(stageOne.secondLightBarrierBefore == 1)
        {
            stageOne.secondLightBarrierBefore = 0;
            stageOne.hasItemPassedSecondLB = 1;
        }
    }
    else
    {
        stageOne.secondLightBarrierBefore = 1;
    }

    //item left stage?
    i = 0;
    for( i ; i < 3; i++)
    {
        if(stageOne.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED) && stageOne.hasItemPassedSecondLB)
        {
            stageTwo.isOccupied = 1;
            stageOne.itemPositions[i] = -1;
            stageOne.itemCount --;
            stageOne.hasItemPassedSecondLB = 0;
            break;
        }
    }
}

void computeFirstPlate()
{
    // is stage ready?
    if(stageTwo.isOccupied || getFirstPusher()->isBackTriggerActivated != 1)
    {
        stageTwo.isReady = 0;
    }
    else
    {
        stageTwo.isReady = 1;
    }

    if(stageTwo.isOccupied && stageThree.isReady)
    {
        if(getFirstPusher()->isBackTriggerActivated)
        {
            stageTwo.pusherDir = 2; //forwards
        }
    }
    else if(getFirstPusher()->isBackTriggerActivated)
    {
        stageTwo.pusherDir = 1; //incative
    }

    if(getFirstPusher()->isFrontTriggerActivated)
    {
        stageTwo.pusherDir = 0; //backwards
        stageTwo.isOccupied = 0;
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageThree.itemCount + stageFour.itemCount + stageFive.isOccupied + stageSix.itemCount) > 0)
        {
            stageThree.itemCount++;
        }
    }
}

void computeSecondTreadmill()
{
    // check if timeout
    if(getThirdLightBarrier()->isBlocked)
    {
        stageThree.timeout = 0;
    }
    if(stageThree.timeout > 0)
    {
        stageThree.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageThree.timeout >= TIMEOUT)
    {
        timeout(3);
    }

    // check if new item
    if(stageThree.itemCount > stageThree.itemCountBefore)
    {
        // new item
        stageThree.itemCountBefore = stageThree.itemCount;

        int i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -1)
            {
                stageThree.itemPositions[i] = 0;
                break;
            }
        }
    }

    if(stageThree.itemCount > 0)
    {
        if(stageThree.isToolTime)
        {
            if(stageThree.tMRuntimeLeftForCentering <= 0)
            {
                //centered
                stageThree.isTMRunning = 0;
                if(stageThree.toolRuntimeLeft > 0)
                {
                    stageThree.isToolRunning = 1;
                }
                else
                {
                    if(stageFour.isReady)
                    {
                        //done with tooltime
                        stageThree.isToolTime = 0;
                        stageThree.isTMRunning = 1;
                        stageThree.tMRuntimeLeftForCentering = 0;
                        stageFour.timeout = 1;
                        stageThree.isWaiting = 0;
                    }
                    else
                    {
                        stageThree.isWaiting = 1;
                    }
                    stageThree.isToolRunning = 0;
                    stageThree.toolRuntimeLeft = 0;
                }
            }
        }
        else
        {
            int i = 0;
            for( i ; i < 3 ; i++)
            {
                // check if tool time
                if(getThirdLightBarrier()->isBlocked)
                {
                    if(stageThree.lightBarrierBefore == 0)
                    {
                        stageThree.lightBarrierBefore = 1;
                        stageThree.isToolTime = 1;
                        stageThree.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                        stageThree.toolRuntimeLeft = TOOL_TIME;
                        break;
                    }
                }
                else
                {
                    stageThree.lightBarrierBefore = 0;
                    stageThree.isTMRunning = 1;
                    stageThree.hasItemPassedLightBarrier = 1;
                }

                // check if item leaves
                if((stageThree.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED)) && stageThree.hasItemPassedLightBarrier)
                {
                    stageThree.itemCount--;
                    stageThree.itemCountBefore--;
                    stageThree.itemPositions[i] = -1;
                    stageFour.itemCount++;
                    stageThree.hasItemPassedLightBarrier = 0;
                }
            }
        }
    }
    else
    {
        stageThree.isTMRunning = 0;
    }

    //is this stage ready for more items?
    int i = 0;
    if(stageThree.itemCount > 0)
    {
        for( i ; i < 3 ; i++)
        {
            int pos = stageThree.itemPositions[i];
            if((pos >= 0 && pos <= 0 + EMPTY_SPACE_TO_BE_READY) || stageThree.isWaiting)
            {
                stageThree.isReady = 0;
                break;
            }
            else
            {
                stageThree.isReady = 1;
            }
        }
    }
    else
    {
        stageThree.isReady = 1;
    }
}

void computeThirdTreadmill()
{
    // check if timeout
    if(getFourthLightBarrier()->isBlocked)
    {
        stageFour.timeout = 0;
    }
    if(stageFour.timeout > 0)
    {
        stageFour.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageFour.timeout >= TIMEOUT)
    {
        timeout(4);
    }

    // check if new item
    if(stageFour.itemCount > stageFour.itemCountBefore)
    {
        // new item
        stageFour.itemCountBefore = stageFour.itemCount;

        int i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -1)
            {
                stageFour.itemPositions[i] = 0;
                break;
            }
        }
    }

    if(stageFour.itemCount > 0)
    {
        if(stageFour.isToolTime)
        {
            if(stageFour.tMRuntimeLeftForCentering <= 0)
            {
                //centered
                stageFour.isTMRunning = 0;
                if(stageFour.toolRuntimeLeft > 0)
                {
                    stageFour.isToolRunning = 1;
                }
                else
                {
                    //done with tooltime
                    if(stageFive.isReady)
                    {
                        stageFour.isToolTime = 0;
                        stageFour.isTMRunning = 1;
                        stageFour.tMRuntimeLeftForCentering = 0;
                        stageSix.timeout = 1;
                        stageFour.isWaiting = 0;
                    }
                    else
                    {
                        stageFour.isWaiting = 1;
                    }
                    stageFour.isToolRunning = 0;
                    stageFour.toolRuntimeLeft = 0;
                }
            }
        }
        else
        {
            int i = 0;
            for( i ; i < 3 ; i++)
            {
                // check if tool time
                if(getFourthLightBarrier()->isBlocked)
                {
                    if(stageFour.lightBarrierBefore == 0)
                    {
                        stageFour.lightBarrierBefore = 1;
                        stageFour.isToolTime = 1;
                        stageFour.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                        stageFour.toolRuntimeLeft = TOOL_TIME;
                        break;
                    }
                }
                else
                {
                    stageFour.lightBarrierBefore = 0;
                    stageFour.isTMRunning = 1;
                    stageThree.hasItemPassedLightBarrier = 1;
                }

                // check if item leaves
                if((stageFour.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED)) && stageThree.hasItemPassedLightBarrier)
                {
                    stageFour.itemCount--;
                    stageFour.itemCountBefore--;
                    stageFour.itemPositions[i] = -1;

                    // TODO: give it to next stage
                    stageFive.isOccupied = 1;
										stageThree.hasItemPassedLightBarrier = 0;
                }
            }
        }
    }
    else
    {
        stageFour.isTMRunning = 0;
    }

    //is this stage ready for more items?
    int i = 0;
    if(stageFour.itemCount > 0)
    {
        for( i ; i < 3 ; i++)
        {
            int pos = stageFour.itemPositions[i];
            if((pos >= 0 && pos <= 0 + EMPTY_SPACE_TO_BE_READY) || stageFour.isWaiting)
            {
                stageFour.isReady = 0;
                break;
            }
            else
            {
                stageFour.isReady = 1;
            }
        }
    }
    else
    {
        stageFour.isReady = 1;
    }
}

void computeSecondPlate()
{

    // is stage ready?
    if(stageFive.isOccupied || getSecondPusher()->isBackTriggerActivated != 1)
    {
        stageFive.isReady = 0;
    }
    else
    {
        stageFive.isReady = 1;
    }

    if(stageFive.isOccupied && stageThree.isReady)
    {
        if(getSecondPusher()->isBackTriggerActivated)
        {
            stageFive.pusherDir = 2; //forwards
        }
    }
    else if(getSecondPusher()->isBackTriggerActivated)
    {
        stageFive.pusherDir = 1; //incative
    }

    if(getSecondPusher()->isFrontTriggerActivated)
    {
        stageFive.pusherDir = 0; //backwards
        stageFive.isOccupied = 0;
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageTwo.isOccupied + stageThree.itemCount + stageFour.itemCount + stageSix.itemCount) > 0)
        {
            stageSix.itemCount++;
        }
    }
}

void computeFourthTredmill()
{
    // check if timeout
    if(getFifthLightBarrier()->isBlocked)
    {
        stageSix.timeout = 0;
    }
    if(stageSix.timeout > 0)
    {
        stageSix.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageSix.timeout >= TIMEOUT)
    {
        timeout(6);
    }

    if(stageSix.itemCount > 0) // not empty?
    {
        if(stageSix.lightBarrierBlockedTime >= STAGE_SIX_LB_CHECK_FULL_TIME) //to long blocked?
        {
            stageSix.isFull = 1;
            if(stageSix.timeLeftForNextEmptyCheck <= 0) // next check now?
            {
                if(stageSix.tMRuntimeLeftForChecking >= 0) //TM runtime left?
                {
                    stageSix.isRunning = 1;
                    stageSix.tMRuntimeLeftForChecking -= totalSystem.timeDiffSinceLastCall;
                }
                else
                {
                    stageSix.isRunning = 0;
                    stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_CHECK_INTERVAL;
                }
            }
            else
            {
                stageSix.isRunning = 0;
                stageSix.timeLeftForNextEmptyCheck -= totalSystem.timeDiffSinceLastCall;
            }
        }
        else
        {
            stageSix.isFull = 0;
            stageSix.isRunning = 1;
        }
    }
    else
    {
        stageSix.isRunning = 0;
    }

    if(stageSix.isFull)
    {
        if(stageSix.timeLeftForNextEmptyCheck > 0)
        {
            if(stageSix.tMRuntimeLeftForChecking > 0)
            {
                stageSix.tMRuntimeLeftForChecking -= totalSystem.timeDiffSinceLastCall;
                stageSix.isRunning = 1;
            }
            else
            {
                stageSix.isRunning = 0;
            }
        }
        else
        {
            stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_LB_CHECK_FULL_TIME;
            stageSix.tMRuntimeLeftForChecking = -1;
        }
        stageSix.timeLeftForNextEmptyCheck -= totalSystem.timeDiffSinceLastCall;
    }

    //light barrier toggled?
    if(getFifthLightBarrier()->isBlocked == 1)
    {
        stageSix.lightBarrierBlockedTime += totalSystem.timeDiffSinceLastCall;
        if(stageSix.lightBarrierBefore == 0)
        {
            stageSix.lightBarrierBefore = 1;
        }
    }
    else if(stageSix.lightBarrierBefore == 1)
    {
        stageSix.lightBarrierBlockedTime = 0;
        stageSix.lightBarrierBefore = 0;
        //toggled to off
        stageSix.itemCount--;
        totalSystem.itemsInSystem--;
        stageSix.wasFullBefore = 0;
    }
}

void runningHandleActors() {
    Treadmill *treadmills[4] = {getFirstTreadmill(), getSecondTreadmill(), getThirdTreadmill(), getFourthTreadmill()};
    Tool *tools[2] = {getFirstTool(), getSecondTool()};

    //first stage
    if(stageOne.isRunning)
    {
        startTreadmill(treadmills[0]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageOne.itemPositions[i] > -1)
            {
                stageOne.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
    }
    else
    {
        stopTreadmill(treadmills[0]);
    }

    //second stage
    Pusher *firstPusher = getFirstPusher();
    switch(stageTwo.pusherDir)
    {
        case 0: runBackwardsPusher(firstPusher); break;
        case 1: stopPusher(firstPusher); break;
        case 2: runForwardPusher(firstPusher); break;
    }

    //third stage
    if(stageThree.isTMRunning)
    {
        startTreadmill(treadmills[1]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageThree.itemPositions[i] > -1)
            {
                stageThree.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
        if(stageThree.isToolTime)
        {
            stageThree.tMRuntimeLeftForCentering -= totalSystem.timeDiffSinceLastCall;
        }
    }
    else
    {
        stopTreadmill(treadmills[1]);
    }

    if(stageThree.isToolRunning)
    {
        startTool(tools[0]);
        stageThree.toolRuntimeLeft -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stopTool(tools[0]);
    }

    //fourth stage
    if(stageFour.isTMRunning)
    {
        startTreadmill(treadmills[2]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageFour.itemPositions[i] > -1)
            {
                stageFour.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
        if(stageFour.isToolTime)
        {
            stageFour.tMRuntimeLeftForCentering -= totalSystem.timeDiffSinceLastCall;
        }
    }
    else
    {
        stopTreadmill(treadmills[2]);
    }

    if(stageFour.isToolRunning)
    {
        startTool(tools[1]);
        stageFour.toolRuntimeLeft -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stopTool(tools[1]);
    }

    //fifth stage
    Pusher *secondPusher = getSecondPusher();
    switch(stageFive.pusherDir)
    {
        case 0: runBackwardsPusher(secondPusher); break;
        case 1: stopPusher(secondPusher); break;
        case 2: runForwardPusher(secondPusher); break;
    }

    //sixth stage
    if(stageSix.isRunning)
    {
        startTreadmill(getFourthTreadmill());
    }
    else
    {
        stopTreadmill(getFourthTreadmill());
    }
}

void runningComputeActions() {
    computeFirstTreadmill();
    computeFirstPlate();
    computeSecondTreadmill();
    computeThirdTreadmill();
    computeSecondPlate();
    computeFourthTredmill();
}
