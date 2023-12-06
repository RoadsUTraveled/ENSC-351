#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Inclue header files here
#include "common.h"
#include "CartMove.h"

int main()
{
/************************ TEST CODE FOR CartMove ************************/
    initMotorDriver();

    printf("Initializing motor driver...\n");
    sleep(2); // Wait for 2 seconds

    printf("Moving forward...\n");
    moveForward();
    sleep(2); // Move forward for 5 seconds

    printf("Moving backward...\n");
    moveBackward();
    sleep(2); // Move backward for 5 seconds

    printf("Turning right...\n");
    turnRight();
    sleep(2); // Turn right for 3 seconds

    printf("Turning left...\n");
    turnLeft();
    sleep(2); // Turn left for 3 seconds

    clearMotorDriver();
/************************ END OF TEST CODE FOR CartMove ************************/
    return 0;
}