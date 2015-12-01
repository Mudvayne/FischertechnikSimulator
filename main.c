#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#include "model.h"

//for painting
COORD coord = {0, 0};

//speeds
double simulationSpeed = 0.5;
double pusherStepSpeed = 0.025;

//item runtimes
int firstThreadMillRunTime = 0;
int secondThreadMillRunTime = 0;
int thirdThreadMillRunTime = 0;
int fourthThreadMillRunTime = 0;

//pushers
double firstPusherPos = 0;
double secondPusherPos = 0;

//predictions
short predictThreadmillOne = 0;
short predictThreadmillTwo = 0;
short predictThreadmillThree = 0;
short predictThreadmillFour = 0;
short predictPlateOne = 0;
short predictPlateTwo = 0;

//better then system("cls")
void gotoxy (int x, int y)
{
    coord.X = x; coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printSimulationInformations()
{
    gotoxy(0,0);

    printf("                   XXXXX              XXXXX\n");
    printf("                   XXXXX on: %d        XXXXX on: %d\n", getFirstTool()->isRunning, getSecondTool()->isRunning);
    printf("                   XXXXX              XXXXX\n");
    printf("pos: %.2f                                                pos: %.2f\n", firstPusherPos, secondPusherPos);
    printf("dir: %d  +---+ on: %d  +  pre: %d   on: %d  +  pre: %d  +---+ dir: %d\n", getFirstPusher()->runningDirection, getSecondThreadmill()->isRunning,
           predictThreadmillTwo, getThirdThreadmill()->isRunning, predictThreadmillThree, getSecondPusher()->runningDirection);
    printf("tri: %d  |   +--------|--------->--------|--------->+   | tri: %d\n", getFirstPusher()->isTriggerActivated, getSecondPusher()->isTriggerActivated);
    printf("pre: %d  +-+-+        +                  +          +-+-+ pre: %d\n", predictPlateOne, predictPlateTwo);
    printf("          ^          %d                  %d            |\n", getThirdLightBarrier()->isBlocked, getFourthLightBarrier()->isBlocked);
    printf("         +-+ %d                                       |\n", getSecondLightBarrier()->isBlocked);
    printf("          |                                          |\n");
    printf("          |                                          |\n");
    printf("          | on: %d                                    | on: %d\n", getFirstThreadmill()->isRunning, getFourthThreadmill()->isRunning);
    printf("          |pre: %d                                    |pre: %d\n", predictThreadmillOne, predictThreadmillFour);
    printf("          |                                          |\n");
    printf("          |                                          |\n");
    printf("         +-+ %d                                      +-+ %d\n", getFirstLightBarrier()->isBlocked, getFifthLightBarrier()->isBlocked);
    printf("          |                                          |\n");
    printf("          +                                          v\n");

    printf("\nThreadmill 1 item runtime: %d ms                          \n", firstThreadMillRunTime);
    printf("Threadmill 2 item runtime: %d ms                          \n", secondThreadMillRunTime);
    printf("Threadmill 3 item runtime: %d ms                          \n", thirdThreadMillRunTime);
    printf("Threadmill 4 item runtime: %d ms                          \n\n", fourthThreadMillRunTime);
}

int main()
{
    printf("Press RETURN to start the simulator!\n");
    getchar();
    system("cls");


    //Testing
    //first threadmill
    getFirstThreadmill()->isRunning = 1;
    getFirstLightBarrier()->isBlocked = 1;

/*
    //second threadmill
    getSecondThreadmill()->isRunning = 1;
    predictThreadmillTwo = 1;

    //third threadmill
    getThirdThreadmill()->isRunning = 1;

    //fourth threadmill
    getFourthThreadmill()->isRunning = 1;
    predictThreadmillFour = 1;
*/
    while(1)
    {
        Sleep(50);

        if(getFirstLightBarrier()->isBlocked == 1)
        {
            predictThreadmillOne = 1;
        }

        //First Threadmill
        if(predictThreadmillOne == 1 && getFirstThreadmill()->isRunning == 1)
        {
            firstThreadMillRunTime += 50*simulationSpeed;
        }
        else
        {
            firstThreadMillRunTime = 0;
        }

        if(predictThreadmillOne && firstThreadMillRunTime <= 500)
        {
            getFirstLightBarrier()->isBlocked = 1;
        }

        if(predictThreadmillOne && firstThreadMillRunTime > 500 && firstThreadMillRunTime < 3000)
        {
            getFirstLightBarrier()->isBlocked = 0;
        }

        if (predictThreadmillOne && firstThreadMillRunTime >= 3000 && firstThreadMillRunTime <= 4000)
        {
            getSecondLightBarrier()->isBlocked = 1;
        }

        if(predictThreadmillOne && firstThreadMillRunTime > 4000)
        {
            getSecondLightBarrier()->isBlocked = 0;
        }

        if(predictThreadmillOne && firstThreadMillRunTime > 5000)
        {
            if(firstPusherPos > 0)
            {
                system("cls");
                printf("FATAL: First Plate was occupied by the first pusher when an item arrived!");
                getchar();
                return 0;
            }
            if(predictPlateOne == 1)
            {
                system("cls");
                printf("FATAL: First Plate was already occupied by an item!");
                getchar();
                return 0;
            }
            predictThreadmillOne = 0;
            predictPlateOne = 1;
            firstThreadMillRunTime = 0;
        }

        //First Plate
        if(firstPusherPos>=1.0 && firstPusherPos <= 1.1)
        {
            getFirstPusher()->isTriggerActivated = 1;
        }
        else
        {
            getFirstPusher()->isTriggerActivated = 0;
        }

        if(getFirstPusher()->runningDirection == FORWARDS)
        {
            firstPusherPos += pusherStepSpeed*simulationSpeed;
        }
        if(getFirstPusher()->runningDirection == BACKWARDS)
        {
            firstPusherPos -= pusherStepSpeed*simulationSpeed;
        }
        if(firstPusherPos <= -0.2 || firstPusherPos >= 1.2)
        {
            system("cls");
            printf("FATAL: Engine of first pusher damaged!");
            getchar();
            return 0;
        }
        if(firstPusherPos >= 9.0 && predictPlateOne == 1)
        {
            predictPlateOne = 0;
            predictThreadmillTwo = 1;
        }

        //Second Threadmill
        if(predictThreadmillTwo == 1 && getSecondThreadmill()->isRunning == 1)
        {
            secondThreadMillRunTime += 50*simulationSpeed;
        }
        if(predictThreadmillTwo && secondThreadMillRunTime >= 2000 && secondThreadMillRunTime <= 3000)
        {
            getThirdLightBarrier()->isBlocked = 1;
        }
        else
        {
            getThirdLightBarrier()->isBlocked = 0;
        }
        if(predictThreadmillTwo == 1 && secondThreadMillRunTime >= 5000)
        {
            secondThreadMillRunTime = 0;
            predictThreadmillTwo = 0;
            predictThreadmillThree = 1;
            //collision?
        }

        //Third Threadmill
        if(predictThreadmillThree == 1 && getThirdThreadmill()->isRunning == 1)
        {
            thirdThreadMillRunTime += 50*simulationSpeed;
        }
        if(predictThreadmillThree && thirdThreadMillRunTime >= 2000 && thirdThreadMillRunTime <= 3000)
        {
            getFourthLightBarrier()->isBlocked = 1;
        }
        else
        {
            getFourthLightBarrier()->isBlocked = 0;
        }
        if(predictThreadmillThree == 1 && thirdThreadMillRunTime >= 5000)
        {
            if(secondPusherPos > 0)
            {
                system("cls");
                printf("FATAL: Second Plate was occupied by the first pusher when an item arrived!");
                getchar();
                return 0;
            }
            if(predictPlateTwo == 1)
            {
                system("cls");
                printf("FATAL: Second Plate was already occupied by an item!");
                getchar();
                return 0;
            }

            thirdThreadMillRunTime = 0;
            predictThreadmillThree = 0;
            predictPlateTwo = 1;
        }

        //Second Plate
        if(secondPusherPos>=1.0 && secondPusherPos <= 1.1)
        {
            getSecondPusher()->isTriggerActivated = 1;
        }
        else
        {
            getSecondPusher()->isTriggerActivated = 0;
        }

        if(getSecondPusher()->runningDirection == FORWARDS)
        {
            secondPusherPos += pusherStepSpeed*simulationSpeed;
        }
        if(getSecondPusher()->runningDirection == BACKWARDS)
        {
            secondPusherPos -= pusherStepSpeed*simulationSpeed;
        }
        if(secondPusherPos <= -0.2 || secondPusherPos >= 1.2)
        {
            system("cls");
            printf("FATAL: Engine of second pusher damaged!");
            getchar();
            return 0;
        }
        if(secondPusherPos >= 9.0 && predictPlateTwo == 1)
        {
            predictPlateTwo = 0;
            predictThreadmillFour = 1;
        }

        //Fourth Threadmill
        if(predictThreadmillFour == 1 && getFourthThreadmill()->isRunning == 1)
        {
            fourthThreadMillRunTime += 50*simulationSpeed;
        }
        if(predictThreadmillFour && fourthThreadMillRunTime >= 4000 && fourthThreadMillRunTime <= 5000)
        {
            getFifthLightBarrier()->isBlocked = 1;
        }
        else
        {
            getFifthLightBarrier()->isBlocked = 0;
        }
        if(predictThreadmillFour == 1 && fourthThreadMillRunTime >= 5000)
        {
            //TODO
        }

        printSimulationInformations();
    }

    return 0;
}
