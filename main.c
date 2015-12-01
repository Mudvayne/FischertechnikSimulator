#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#include "model.h"

COORD coord = {0, 0};

//item runtimes
int firstThreadMillRunTime = 0;
int secondThreadMillRunTime = 0;
int thirdThreadMillRunTime = 0;
int fourthThreadMillRunTime = 0;

//pusher positions
double firstPusherPos = 0;
double secondPusherPos = 0;

//predictions
short predictThreadmillOne = 0;
short predictThreadmillTwo = 0;
short predictThreadmillThree = 0;
short predictThreadmillFour = 0;
short predictPlateOne = 0;
short predictPlateTwo = 0;


void gotoxy (int x, int y)
{
    coord.X = x; coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printSimulatorInfos()
{
    gotoxy(0,0);
    printf("Threadmills:                                                  \n");
    printf("FIRST:\t RUNNING: %d, RUNTIME: %d ms                          \n", getFirstThreadmill()->isRunning, firstThreadMillRunTime);
    printf("SECOND:\t RUNNING: %d, RUNTIME: %d ms                          \n", getSecondThreadmill()->isRunning, secondThreadMillRunTime);
    printf("THIRD:\t RUNNING: %d, RUNTIME: %d ms                          \n", getThirdThreadmill()->isRunning, thirdThreadMillRunTime);
    printf("FOURTH:\t RUNNING: %d, RUNTIME: %d ms                          \n\n", getFourthThreadmill()->isRunning, fourthThreadMillRunTime);

    printf("Pushers:\n");
    printf("FIRST:\t DIRECTION: %d, POS: %.2f, TRIGGER: %d                          \n", getFirstPusher()->runningDirection, firstPusherPos, getFirstPusher()->isTriggerActivated);
    printf("SECOND:\t DIRECTION: %d, POS: %.2f, TRIGGER: %d                          \n\n", getSecondPusher()->runningDirection, secondPusherPos, getSecondPusher()->isTriggerActivated);

    printf("Tools:\t\t\t %d, %d                           \n",
           getFirstTool()->isRunning,
           getSecondTool()->isRunning);

    printf("Light Barriers:\t\t %d, %d, %d, %d, %d                          \n\n",
           getFirstLightBarrier()->isBlocked,
           getSecondLightBarrier()->isBlocked,
           getThirdLightBarrier()->isBlocked,
           getFourthLightBarrier()->isBlocked,
           getFifthLightBarrier()->isBlocked);

    // Item prediction
    printf("Item Prediction:\n");
    if(predictThreadmillOne) printf("There should be an item on threadmill 1.                          \n");
    if(predictThreadmillTwo) printf("There should be an item on threadmill 2.                          \n");
    if(predictThreadmillThree) printf("There should be an item on threadmill 3.                          \n");
    if(predictThreadmillFour) printf("There should be an item on threadmill 4.                          \n");
    if(predictPlateOne) printf("There should be an item on plate 1.                          \n");
    if(predictPlateTwo) printf("There should be an item on plate 2.                          \n");

    int linesToDelete = 6 - predictThreadmillOne + predictThreadmillTwo + predictThreadmillThree + predictThreadmillFour + predictPlateOne + predictPlateTwo;
    int i=0;
    for( i ; i < linesToDelete ; i++)
    {
        printf("                                                    ");
    }
}

int main()
{
    printf("Press ENTER key to start the simulator!\n");
    getchar();

    //Testing
    getFirstThreadmill()->isRunning=1;
    short newItem = 1;

    short notifyWhenSensed = 0;

    short firstRun = 1;

    while(1)
    {
        Sleep(500);

        //First Threadmill
        if(firstRun == 1 || (predictThreadmillOne == 1 && getFirstThreadmill()->isRunning == 1))
        {
            firstThreadMillRunTime += 100;
            firstRun = 0;
        }
        else
        {
            firstThreadMillRunTime = 0;
        }

        if(firstThreadMillRunTime > 0 && firstThreadMillRunTime <= 5000)
        {
            predictThreadmillOne = 1;
        }

        if(firstThreadMillRunTime <= 500 && newItem == 1)
        {
            getFirstLightBarrier()->isBlocked = 1;
        }

        if(firstThreadMillRunTime > 500 && firstThreadMillRunTime < 3000)
        {
            getFirstLightBarrier()->isBlocked = 0;
            newItem = 0;
        }

        if(firstThreadMillRunTime >= 3000 && firstThreadMillRunTime <= 4000)
        {
            getSecondLightBarrier()->isBlocked = 1;
        }

        if(firstThreadMillRunTime > 4000)
        {
            getSecondLightBarrier()->isBlocked = 0;
        }

        if(firstThreadMillRunTime > 5000)
        {
            predictThreadmillOne = 0;
            predictPlateOne = 1;
        }

        //First Plate
        if(getFirstPusher()->runningDirection == FORWARDS)
        {
            firstPusherPos += 0.025;
        }
        if(getFirstPusher()->runningDirection == BACKWARDS)
        {
            firstPusherPos -= 0.025;
        }

        //Todo


        printSimulatorInfos();
    }
    return 0;
}
