#include "program.h"

void timeout(short stage)
{
    /*
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
	*/
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
            short i = 0;
            for( i ; i < 3; i++)
            {
                if(stageOne.itemPositions[i] == -1)
                {
                    stageOne.itemPositions[i] = -2; //position will be set at second LB
                    break;
                }
            }
        }
    }
    else if(stageOne.firstLightBarrierBefore == 1)
    {
        //item left first light barrier
        stageOne.firstLightBarrierBefore = 0;
        //TODO: Timeout
        stageOne.timeout += 1;
    }

    if(stageOne.waitTime > 0)
    {
        stageOne.waitTime -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stageOne.waitTime = 0;
    }

    //has a item passed LB2?
    if(getSecondLightBarrier()->isBlocked == 0)
    {
        if(stageOne.secondLightBarrierBefore == 1)
        {
            // item left LB2
            stageOne.secondLightBarrierBefore = 0;
            stageOne.hasItemPassedSecondLB = 1;
            stageOne.waitTime = STAGE_ONE_COOLDOWN_AFTER_SECOND_LB;
            stageOne.messureTimeBetweenItems = 1;
        }
    }
    else
    {
        if(stageOne.secondLightBarrierBefore == 0)
        {
            //new item
            stageOne.secondLightBarrierBefore = 1;

            //update position to correct value
            short i = 0;
            for( i ; i < 3 ; i++)
            {
                if(stageOne.itemPositions[i] == -2)
                {
                    stageOne.itemPositions[i] = TM1_RUNTIME * LB2_POS;
                    break;
                }
            }
        }
    }

    //should it run?
    stageOne.isRunning = 0;
    if(stageOne.itemCount > 0)
    {
        stageOne.isRunning = 1;
        if(getSecondLightBarrier()->isBlocked && stageTwo.isReady == 0)
        {
            stageOne.isRunning = 0;
        }
    }

    //item left stage?
    short i = 0;
    for( i ; i < 3; i++)
    {
        if((stageOne.itemPositions[i] > TM1_RUNTIME) && stageOne.hasItemPassedSecondLB)
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
    //is stage ready?
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
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageThree.itemCount + stageFour.itemCount + stageFive.isOccupied + stageSix.itemCount) > 0) //do not "generate" items
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

        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -1)
            {
                stageThree.itemPositions[i] = -2;
                break;
            }
        }
    }

    stageThree.isReady = 1;

    if(stageThree.itemCount > 0)
    {
        // RDY?
        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -2)
            {
                stageThree.isReady = 0;
                break;
            }
        }

        // TM and TOOL
        if(stageThree.isToolTime)
        {
            if(stageThree.tMRuntimeLeftForCentering > 0)
            {
                stageThree.isTMRunning = 1;
            }
            else
            {
                if(stageThree.toolRuntimeLeft > 0)
                {
                    //Tooling
                    stageThree.isToolRunning = 1;
                    stageThree.isTMRunning = 0;
                }
                else
                {
                    //Done Tooltime
                    stageThree.isToolTime = 0;
                    stageThree.isToolRunning = 0;
                    stageThree.isTMRunning = 1;
                    stageThree.toolRuntimeLeft = 0;
                    stageFour.timeout = 1;
                }

                //update position if not already done
                if(stageThree.updatedPosInToolTime == 0)
                {
                    short i = 0;
                    for( i ; i < 3 ; i++)
                    {
                        if(stageThree.itemPositions[i] == -2)
                        {
                            stageThree.itemPositions[i] = TM2_RUNTIME * TOOL_LB_POS;
                            break;
                        }
                    }
                    stageThree.updatedPosInToolTime = 1;
                }
            }
        }
        else
        {
            if(getThirdLightBarrier()->isBlocked)
            {
                if(stageThree.lightBarrierBefore == 0)
                {
                    //new item in light barrier
                    stageThree.lightBarrierBefore = 1;
                    stageThree.isToolTime = 1;
                    stageThree.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                    stageThree.toolRuntimeLeft = MILL_TIME;
                    stageThree.updatedPosInToolTime = 0;
                }
            }
            else
            {
                if(stageThree.lightBarrierBefore = 1)
                {
                    //item leaves light barrier
                    stageThree.hasItemPassedLightBarrier = 1;
                }
                // light barrier not blocked
                stageThree.lightBarrierBefore = 0;
            }
            stageThree.isTMRunning = 1;
        }
    }
    else
    {
        stageThree.isTMRunning = 0;
        stageThree.isToolTime = 0;
        stageThree.toolRuntimeLeft = 0;
        stageThree.tMRuntimeLeftForCentering = 0;
    }

    //should it wait for next station or traverse item?
    short i = 0;
    for( i ; i < 3 ; i++)
    {
        //wait for next stage?
        if((stageThree.itemPositions[i] >= (TM2_RUNTIME * TOOL_STAGE_WAIT_POS)) && stageFour.isReady == 0)
        {
            stageThree.isTMRunning = 0;
            stageFour.timeout = -1;
            break;
        }

        //does item leave stage?
        if(stageThree.itemPositions[i] > TM2_RUNTIME)
        {
            stageThree.itemCount--;
            stageThree.itemCountBefore--;
            stageThree.itemPositions[i] = -1;
            stageFour.itemCount++;
            stageThree.hasItemPassedLightBarrier = 0;
        }
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
        timeout(3);
    }

    // check if new item
    if(stageFour.itemCount > stageFour.itemCountBefore)
    {
        // new item
        stageFour.itemCountBefore = stageFour.itemCount;

        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -1)
            {
                stageFour.itemPositions[i] = -2;
                break;
            }
        }
    }

    stageFour.isReady = 1;

    if(stageFour.itemCount > 0)
    {
        // RDY?
        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -2)
            {
                stageFour.isReady = 0;
                break;
            }
        }

        // TM and TOOL
        if(stageFour.isToolTime)
        {
            if(stageFour.tMRuntimeLeftForCentering > 0)
            {
                stageFour.isTMRunning = 1;
            }
            else
            {
                if(stageFour.toolRuntimeLeft > 0)
                {
                    //Tooling
                    stageFour.isToolRunning = 1;
                    stageFour.isTMRunning = 0;
                }
                else
                {
                    //Done Tooltime
                    stageFour.isToolTime = 0;
                    stageFour.isToolRunning = 0;
                    stageFour.isTMRunning = 1;
                    stageFour.toolRuntimeLeft = 0;
                    stageSix.timeout = 1;
                }

                //update position if not already done
                if(stageFour.updatedPosInToolTime == 0)
                {
                    short i = 0;
                    for( i ; i < 3 ; i++)
                    {
                        if(stageFour.itemPositions[i] == -2)
                        {
                            stageFour.itemPositions[i] = TM3_RUNTIME * TOOL_LB_POS;
                            break;
                        }
                    }
                    stageFour.updatedPosInToolTime = 1;
                }
            }
        }
        else
        {
            if(getFourthLightBarrier()->isBlocked)
            {
                if(stageFour.lightBarrierBefore == 0)
                {
                    //new item in light barrier
                    stageFour.lightBarrierBefore = 1;
                    stageFour.isToolTime = 1;
                    stageFour.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                    stageFour.toolRuntimeLeft = MILL_TIME;
                    stageFour.updatedPosInToolTime = 0;
                }
            }
            else
            {
                if(stageFour.lightBarrierBefore = 1)
                {
                    //item leaves light barrier
                    stageFour.hasItemPassedLightBarrier = 1;
                }
                // light barrier not blocked
                stageFour.lightBarrierBefore = 0;
            }
            stageFour.isTMRunning = 1;
        }
    }
    else
    {
        stageFour.isTMRunning = 0;
        stageFour.isToolTime = 0;
        stageFour.toolRuntimeLeft = 0;
        stageFour.tMRuntimeLeftForCentering = 0;
    }

    //should it wait for next station or traverse item?
    short i = 0;
    for( i ; i < 3 ; i++)
    {
        //wait for next stage?
        if((stageFour.itemPositions[i] >= (TM3_RUNTIME * TOOL_STAGE_WAIT_POS)) && stageFive.isReady == 0)
        {
            stageFour.isTMRunning = 0;
            stageSix.timeout = -1;
            break;
        }

        //does item leave stage?
        if(stageFour.itemPositions[i] > TM3_RUNTIME)
        {
            stageFour.itemCount--;
            stageFour.itemCountBefore--;
            stageFour.itemPositions[i] = -1;
            stageFive.isOccupied = 1;
        }
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

    if(stageFive.isOccupied && stageSix.isFull == 0)
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
    stageSix.isRunning = 1;

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

    if(stageSix.itemCount > 0)
    {
        if(stageSix.lightBarrierBlockedTime >= STAGE_SIX_LB_BLOCK_FOR_FULL)
        {
            //stage is full
            if(stageSix.isChecking == 0)
            {
                stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_CHECK_INTERVAL;
                stageSix.tMRuntimeLeftForChecking = STAGE_SIX_RUN_TM_FOR_CHECK_TIME;
            }
            stageSix.isFull = 1;
            stageSix.isChecking = 1;
        }
        else
        {
            stageSix.isFull = 0;
        }
    }
    else
    {
        stageSix.isRunning = 0;
    }

    if(stageSix.isChecking)
    {
        if(stageSix.timeLeftForNextEmptyCheck > 0)
        {
            stageSix.timeLeftForNextEmptyCheck -= totalSystem.timeDiffSinceLastCall;
            stageSix.isRunning = 0;
        }
        else
        {
            //we are in the middle of a check
            if(stageSix.tMRuntimeLeftForChecking < 0)
            {
                stageSix.isRunning = 0;
                stageSix.isChecking = 0;
                stageSix.tMRuntimeLeftForChecking = 0;
                stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_CHECK_INTERVAL;
            }
            else
            {
                stageSix.tMRuntimeLeftForChecking -= totalSystem.timeDiffSinceLastCall;
            }
        }
    }

    //light barrier toggled and blocked time?
    if(getFifthLightBarrier()->isBlocked == 1)
    {
        stageSix.lightBarrierBlockedTime += totalSystem.timeDiffSinceLastCall;
        if(stageSix.lightBarrierBlockedTime >= STAGE_SIX_LB_BLOCK_FOR_FULL)
        {
            stageSix.lightBarrierBlockedTime = STAGE_SIX_LB_BLOCK_FOR_FULL;
        }
        if(stageSix.lightBarrierBefore == 0)
        {
            stageSix.lightBarrierBefore = 1;
        }
    }
    else if(stageSix.lightBarrierBefore == 1)
    {
        stageSix.isFull = 0;
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
        short i = 0;
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
        short i = 0;
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
        short i = 0;
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