#include "elev.h"
#include "liftCtrl.h"
#include "hwInterface.h"
#include "doorTimer.h"
#include <stdio.h>
#include <stdbool.h>

void reInitializeElev();
void printElevStatus();

int main() {
 
    // Initialize hardware
    if (!elevInit()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    // Sets elevator in defined floor
    if (elevGetFloorSensor() == -1) {
        elevSetMotorDir(DIRN_UP);
    }

    bool initialized = false;

    while(!initialized) {
        if (elevGetFloorSensor() != -1) {  
            reInitializeElev();
            initialized = true;
        }  
    }

    // Main loop
    while(1) {

        // Trigger stateFloor
        if (elevGetFloorSensor() == hwInterfaceGetNextFloor()) {
            printf("Arrived at destination\n");
            stateFloor();

            // Trigger stop
            if(elevGetStopSignal()) {
                stateStop();
            }
        }
        printElevStatus();

        // Trigger stateMove
        if ((hwInterfaceGetNumberOfOrders() > 0)) { 
            printf("Drive\n");
            stateMove();

            // Trigger stop
            if(elevGetStopSignal()) { 
                stateStop();
            }
        }    
    }
    return 0;
}

/* ----------- Utilities ------------ */
void reInitializeElev() {
    hwInterfaceSetCurrentFloor(elevGetFloorSensor());
    hwInterfaceSetNextFloor(hwInterfaceGetCurrentFloor());
    hwInterfaceSetLastDir(DIRN_UP);
    elevSetMotorDir(0);
    hwInterfaceSetCurrentDir(0);                               

    printCurrentFloor();
    printNextFloor();
}

void printElevStatus() {
    printCurrentFloor();
    printNextFloor();
    printf("\n");
    printMatrix();
    printf("\n");

    printf("Amount of orders: %d \n", hwInterfaceGetNumberOfOrders());
}
