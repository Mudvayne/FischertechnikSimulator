#import "simulator.h"

//for painting
COORD coord = {0, 0};

//speeds
double msPerSimStep = 5;
double simulationSpeed = 0.2;
double pusherStepSpeed = 0.015;
double treadmillStepSpeed = 50;

//item runtimes
int firstTreadMillRunTime = 0;
int secondTreadMillRunTime = 0;
int thirdTreadMillRunTime = 0;
int fourthTreadMillRunTime = 0;

//pushers
double firstPusherPos = 0;
double secondPusherPos = 0;

//predictions
short predictTreadmillOne = 0;
short predictTreadmillTwo = 0;
short predictTreadmillThree = 0;
short predictTreadmillFour = 0;
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

    printf("                   XXXXX              XXXXX         \n");
    printf("                   XXXXX on: %d        XXXXX on: %d         \n", getFirstTool()->isRunning, getSecondTool()->isRunning);
    printf("                   XXXXX              XXXXX         \n");
    printf("pos: %.2f                                                pos: %.2f\n", firstPusherPos, secondPusherPos);
    printf("dir: %d  +---+ on: %d  +  pre: %d   on: %d  +  pre: %d  +---+ dir: %d\n", getFirstPusher()->runningDirection, getSecondTreadmill()->isRunning,
           predictTreadmillTwo, getThirdTreadmill()->isRunning, predictTreadmillThree, getSecondPusher()->runningDirection);
    printf("trf: %d  |   +--------|--------->--------|--------->+   | trf: %d\n", getFirstPusher()->isFrontTriggerActivated, getSecondPusher()->isFrontTriggerActivated);
    printf("trb: %d  +-+-+        +                  +          +-+-+ trb: %d\n", getFirstPusher()->isBackTriggerActivated, getSecondPusher()->isBackTriggerActivated);
    printf("pre: %d    ^          %d                  %d            |   pre: %d\n", predictPlateOne, getThirdLightBarrier()->isBlocked, getFourthLightBarrier()->isBlocked, predictPlateTwo);
    printf("         +-+ %d                                       |\n", getSecondLightBarrier()->isBlocked);
    printf("          |                                          |\n");
    printf("          |                                          |\n");
    printf("          | on: %d                                    | on: %d\n", getFirstTreadmill()->isRunning, getFourthTreadmill()->isRunning);
    printf("          |pre: %d                                    |pre: %d\n", predictTreadmillOne, predictTreadmillFour);
    printf("          |                                          |\n");
    printf("          |                                          |\n");
    printf("         +-+ %d                                      +-+ %d\n", getFirstLightBarrier()->isBlocked, getFifthLightBarrier()->isBlocked);
    printf("          |                                          |\n");
    printf("          +                                          v\n");

    printf("\nTreadmill 1 item runtime: %d ms                          \n", firstTreadMillRunTime);
    printf("Treadmill 2 item runtime: %d ms                          \n", secondTreadMillRunTime);
    printf("Treadmill 3 item runtime: %d ms                          \n", thirdTreadMillRunTime);
    //printf("Treadmill 4 item runtime: %d ms                          \n\n", fourthTreadMillRunTime);
}

void simulate()
{
    //printf("Press 1-6 to insert a item into the System.");
    if(kbhit())
    {
        fseek(stdin,0,SEEK_END);

        system("cls");
        printf("INSERT ITEM\n\n");
        printf("1\tFirst Threadmill \n");
        printf("2\tFirst Plate \n");
        printf("3\tSecond Threadmill \n");
        printf("4\tThird Threadmill \n");
        printf("5\tSecond Plate \n");
        printf("6\tFourth Threadmill \n");

        int input = 0;
        scanf("%d",&input);

        switch(input)
        {
            case 1: getFirstLightBarrier()->isBlocked = 1; break;
            case 2: predictPlateOne = 1; break;
            case 3: predictTreadmillTwo = 1; break;
            case 4: predictTreadmillThree = 1; break;
            case 5: predictPlateTwo = 1; break;
            case 6: predictTreadmillFour = 1; break;
            default: printf("\nOnly inputs 1-6 allowed."); getchar();
        }
    }


    Sleep(msPerSimStep);

    if(getFirstLightBarrier()->isBlocked == 1)
    {
        predictTreadmillOne = 1;
    }

    //First Threadmill
    if(predictTreadmillOne == 1 && getFirstTreadmill()->isRunning == 1)
    {
        firstTreadMillRunTime += treadmillStepSpeed*simulationSpeed;
    }
    else if(predictTreadmillOne != 1)
    {
        firstTreadMillRunTime = 0;
    }

    if(predictTreadmillOne && firstTreadMillRunTime <= 500)
    {
        getFirstLightBarrier()->isBlocked = 1;
    }

    if(predictTreadmillOne && firstTreadMillRunTime > 500 && firstTreadMillRunTime < 3000)
    {
        getFirstLightBarrier()->isBlocked = 0;
    }

    if(predictTreadmillOne && firstTreadMillRunTime >= 3000 && firstTreadMillRunTime <= 4000)
    {
        getSecondLightBarrier()->isBlocked = 1;
    }

    if(predictTreadmillOne && firstTreadMillRunTime > 4000)
    {
        getSecondLightBarrier()->isBlocked = 0;
    }

    if(predictTreadmillOne && firstTreadMillRunTime >= 5000)
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
        predictTreadmillOne = 0;
        predictPlateOne = 1;
        firstTreadMillRunTime = 0;
    }

    //First Plate
    if(firstPusherPos>=1.0 && firstPusherPos <= 1.1)
    {
        getFirstPusher()->isFrontTriggerActivated = 1;
    }
    else
    {
        getFirstPusher()->isFrontTriggerActivated = 0;
    }

    if(firstPusherPos >= -0.1 && firstPusherPos <= 0.0)
    {
        getFirstPusher()->isBackTriggerActivated = 1;
    }
    else
    {
        getFirstPusher()->isBackTriggerActivated = 0;
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
    if(firstPusherPos >= 0.9 && predictPlateOne == 1)
    {
        predictPlateOne = 0;
        predictTreadmillTwo = 1;
    }

    //Second Threadmill
    if(predictTreadmillTwo == 1 && getSecondTreadmill()->isRunning == 1)
    {
        secondTreadMillRunTime += treadmillStepSpeed*simulationSpeed;
    }
    if(predictTreadmillTwo && secondTreadMillRunTime >= 2000 && secondTreadMillRunTime <= 3000)
    {
        getThirdLightBarrier()->isBlocked = 1;
    }
    else
    {
        getThirdLightBarrier()->isBlocked = 0;
    }
    if(predictTreadmillTwo == 1 && secondTreadMillRunTime >= 5000)
    {
        secondTreadMillRunTime = 0;
        predictTreadmillTwo = 0;
        predictTreadmillThree = 1;
        //TODO: collision?
    }

    //Third Threadmill
    if(predictTreadmillThree == 1 && getThirdTreadmill()->isRunning == 1)
    {
        thirdTreadMillRunTime += treadmillStepSpeed*simulationSpeed;
    }
    if(predictTreadmillThree && thirdTreadMillRunTime >= 2000 && thirdTreadMillRunTime <= 3000)
    {
        getFourthLightBarrier()->isBlocked = 1;
    }
    else
    {
        getFourthLightBarrier()->isBlocked = 0;
    }
    if(predictTreadmillThree == 1 && thirdTreadMillRunTime >= 5000)
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

        thirdTreadMillRunTime = 0;
        predictTreadmillThree = 0;
        predictPlateTwo = 1;
    }

    //Second Plate
    if(secondPusherPos>=1.0 && secondPusherPos <= 1.1)
    {
        getSecondPusher()->isFrontTriggerActivated = 1;
    }
    else
    {
        getSecondPusher()->isFrontTriggerActivated = 0;
    }
    if(secondPusherPos >= -0.1 && secondPusherPos <= 0.0)
    {
        getSecondPusher()->isBackTriggerActivated = 1;
    }
    else
    {
        getSecondPusher()->isBackTriggerActivated = 0;
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
    if(secondPusherPos >= 0.9 && predictPlateTwo == 1)
    {
        predictPlateTwo = 0;
        predictTreadmillFour = 1;
    }

    //Fourth Threadmill
    if(predictTreadmillFour == 1 && getFourthTreadmill()->isRunning == 1)
    {
        fourthTreadMillRunTime += treadmillStepSpeed*simulationSpeed;
    }

    if(predictTreadmillFour && fourthTreadMillRunTime >= 4000 && fourthTreadMillRunTime <= 5000)
    {
        getFifthLightBarrier()->isBlocked = 1;
    }
    else
    {
        getFifthLightBarrier()->isBlocked = 0;
    }
    if(predictTreadmillFour == 1 && fourthTreadMillRunTime >= 5000)
    {
        //TODO "voll"
    }

    printSimulationInformations();
}
