#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "common.h"
#include "user_button_exit.h"
#include "segDisplay.h"
#include <stdbool.h> // For bool, true, false
#include <unistd.h>  // For usle
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h> 

int readButton(void) {
    char command[256];
    snprintf(command, sizeof(command), "cat %s", BUTTON);
    FILE *pipe = popen(command, "r");
    char buffer[10];
    if (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) 
            return 0;
    }
    pclose(pipe);
    return atoi(buffer);
}

volatile bool exitRequested = false;

void* buttonMonitorThreadFunc(void* arg) {
    // Initialize the button GPIO
    runCommand("config-pin p8.43 gpio");

    while (!exitRequested) {
        int buttonState = readButton();
        if (buttonState == 0) {
            exitRequested = true;
            printf("pressed button\n");
            ledClose();
            break;
        }
        struct timespec req, rem;
        req.tv_sec = 0;
        req.tv_nsec = 100000000L; // 100 ms in nanoseconds
        nanosleep(&req, &rem);
    }
    return NULL;
}

void controlButtonMonitorThread() {
    pthread_t buttonMonitorThread;

    // Create the button monitor thread
    if (pthread_create(&buttonMonitorThread, NULL, buttonMonitorThreadFunc, NULL) != 0) {
        perror("Failed to create button monitor thread");
        return; // Optionally handle the error as needed
    }

    // Join the button monitor thread
    if (pthread_join(buttonMonitorThread, NULL) != 0) {
        perror("Failed to join button monitor thread");
        return; // Optionally handle the error as needed
    }
}