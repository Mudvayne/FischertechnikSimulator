#include <stdio.h>
#include <stdlib.h>

#include "program.h"
#import "simulator.h"




int main()
{
    system("cls");

    while(1)
    {
        simulate();
        executeProgram();
    }
    return 0;
}
