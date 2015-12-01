#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#include "model.h"

int firstThreadMillRunTime = 0;
int secondThreadMillRunTime = 0;
int thirdThreadMillRunTime = 0;
int fourthThreadMillRunTime = 0;

void printSimulatorInfos()
{


    printf("Threadmills: \n");
    printf("FIRST:\t RUNNING: %d, RUNTIME: %d ms\n", getFirstThreadmill()->isRunning, firstThreadMillRunTime);
    printf("SECOND:\t RUNNING: %d, RUNTIME: %d ms\n", getSecondThreadmill()->isRunning, secondThreadMillRunTime);
    printf("THIRD:\t RUNNING: %d, RUNTIME: %d ms\n", getThirdThreadmill()->isRunning, thirdThreadMillRunTime);
    printf("FOURTH:\t RUNNING: %d, RUNTIME: %d ms\n\n", getFourthThreadmill()->isRunning, fourthThreadMillRunTime);

    printf("Pushers:\n");
    printf("FIRST:\t DIRECTION: %d, TRIGGER: %d\n", getFirstPusher()->runningDirection, getFirstPusher()->isTriggerActivated);
    printf("SECOND:\t DIRECTION: %d, TRIGGER: %d\n\n", getSecondPusher()->runningDirection, getSecondPusher()->isTriggerActivated);

    printf("Tools:\t\t\t %d, %d \n",
           getFirstTool()->isRunning,
           getSecondTool()->isRunning);

    printf("Light Barriers:\t\t %d, %d, %d, %d, %d",
           getFirstLightBarrier()->isBlocked,
           getSecondLightBarrier()->isBlocked,
           getThirdLightBarrier()->isBlocked,
           getFourthLightBarrier()->isBlocked,
           getFifthLightBarrier()->isBlocked);
}

int main()
{
    printf("Press ENTER key to start the simulator!\n");
    getchar();

    //Testing
    getFirstLightBarrier()->isBlocked=1;
    getFirstThreadmill()->isRunning=1;

    short notifyWhenSensed = 1;

    short firstLightBarrierBefore = 0;
    short secondLightBarrierBefore = 0;
    short thirdLightBarrierBefore = 0;
    short fourthLightBarrierBefore = 0;
    short fifthLightBarrierBefore = 0;

    while(1)
    {
        Sleep(10);

        if(kbhit())
        {
            printf("\n\nEnter LightBarrier to activate: ");
            int c = getchar();
            switch(c)
            {
                case '1':   getFirstLightBarrier()->isBlocked = 1;

                            break;
                case '2': break;
                case '3': break;
                case '4': break;
                case '5': break;
                default: printf("\nNO SUCH LIGHT BARRIER!");
            }
        }

        if(getFirstThreadmill()->isRunning == 1)
        {
            if(firstLightBarrierBefore == 0 && notifyWhenSensed == 1)
            {
                printSimulatorInfos();

                printf("\n\nSENSED BRICK IN FIRST LIGHT BARRIER!");
                firstLightBarrierBefore = 1;
                getchar();
            }
            firstThreadMillRunTime += 10;
        }

        time_t start = time(NULL);

        //First Threadmill and Sensors
        if(getFirstLightBarrier()->isBlocked == 1 &&
           firstThreadMillRunTime > 500 &&
           firstThreadMillRunTime < 3000)
        {
            //block leaves first light barrier
            getFirstLightBarrier()->isBlocked = 0;
        }

        if(firstThreadMillRunTime > 3000 && firstThreadMillRunTime < 4000)
        {
            //block enters second light barrier
            if(secondLightBarrierBefore == 0 && notifyWhenSensed == 1)
            {
                printf("\n\nSENSED BRICK IN SECOND LIGHT BARRIER!");
                secondLightBarrierBefore = 1;
                getchar();
            }
            getSecondLightBarrier()->isBlocked = 1;
        }

        if(firstThreadMillRunTime > 4000)
        {
            //block leaves second light barrier
            getSecondLightBarrier()->isBlocked = 0;
            secondLightBarrierBefore = 0;
        }

        system("cls");
        printSimulatorInfos();
    }
    return 0;
}
