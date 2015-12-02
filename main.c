#include <stdio.h>
#include <stdlib.h>

#include "program.h"
#import "simulator.h"




int main()
{
    printf("Press RETURN to start the simulator!\n");
    getchar();
    system("cls");

/*
    //Testing
    //first threadmill
    getFirstTreadmill()->isRunning = 1;
    getFirstLightBarrier()->isBlocked = 1;

    //second threadmill
    getSecondTreadmill()->isRunning = 1;
    predictTreadmillTwo = 1;

    //third threadmill
    getThirdTreadmill()->isRunning = 1;

    //fourth threadmill
    getFourthTreadmill()->isRunning = 1;
    predictTreadmillFour = 1;
*/
    getFirstLightBarrier()->isBlocked = 1;

    while(1)
    {
        simulate();
        executeProgram();
    }
    return 0;
}
