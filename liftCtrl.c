#include "liftCtrl.h"
#include "io.h"
#include "elev.h"
#include "channels.h"

#include "doorTimer.h"
#include "hwInterface.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int stateMove() {

	elevSetDoorOpenLamp(0);
	hwInterfaceSetNextFloorPriority(); 

	if (hwInterfaceGetNextFloor() - hwInterfaceGetCurrentFloor() > 0) { //Set dir up if next > current
		elevSetMotorDir(DIRN_UP);
		hwInterfaceSetCurrentDir(DIRN_UP);

	} else if(hwInterfaceGetNextFloor() - hwInterfaceGetCurrentFloor() < 0) { //set dir down if next < current	
		elevSetMotorDir(DIRN_DOWN);
		hwInterfaceSetCurrentDir(DIRN_DOWN);

	} else { //Stop motor since next is current
		elevSetMotorDir(0);
		hwInterfaceSetCurrentDir(0);
		return 0;
	}

	while(elevGetFloorSensor() != hwInterfaceGetNextFloor()) { // Run this as long as not at right floor

		if(elevGetStopSignal()) { //Brake loop if stopButton is pressed
			return 0;
		}

		int floor_sensor = elevGetFloorSensor();	//Update floor light as elevator is moving.
		if(floor_sensor != -1) {
			elevSetFloorIndicator(floor_sensor);
			hwInterfaceSetCurrentFloor(floor_sensor);
			
		} 
		hwInterfaceCheckOrders(); //Check for orders as elevator is moving.
		hwInterfaceSetNextFloorPriority();
		
	}

	//Elevator is at the right floor, and we want to brake out of the state.
	hwInterfaceSetCurrentFloor(elevGetFloorSensor());
	hwInterfaceSetLastDir(hwInterfaceGetCurrentDir());
	elevSetMotorDir(0);
	printCurrentFloor();

	return 0;
}

void stateFloor() {

	printf("Entered arrive: \n");
	elevSetFloorIndicator(hwInterfaceGetCurrentFloor());

	hwInterfaceSetCurrentDir(0); //Set current dir to 0 when elevator is stalling

	
	hwInterfaceDeleteAllOrders(); //delete orders at floor elevator stops at
	
	//Start timer sequens

	elevSetDoorOpenLamp(1);
	
	startTimer();
	while (!wait3Seconds()) { 
		if(elevGetStopSignal()){ //Brake if stopButton is pressed
			return;
		}
		hwInterfaceCheckOrders();
		hwInterfaceDeleteAllOrders();
	}
	
	elevSetDoorOpenLamp(0);

	//End timer sequens

	while(hwInterfaceGetNumberOfOrders() == 0) { //Poll for order until new order or stopButton

		if(elevGetStopSignal()){
			return;
		}

		if (hwInterfaceCheckOrders()){
			break;
		}
	}
	hwInterfaceSetNextFloorPriority();
}

void stateStop(){

	//Delete all orders and restart elevator.

	int last_dir = hwInterfaceGetCurrentDir();
	hwInterfaceSetCurrentDir(0);
	hwInterfaceSetNextFloor(hwInterfaceGetCurrentFloor());
	elevSetMotorDir(0);
	elevSetStopLamp(1);
	hwInterfaceResetOrders();

	printMatrix();

	if (elevGetFloorSensor() != -1){ //If elev is at a floor, it wil sett arrived_floor after stopButton is pressed
		while(elevGetStopSignal()){
			continue;
		}
		elevSetStopLamp(0);
		return;

	} else { // if bewteen floors
		while(elevGetStopSignal()) {
			continue;
		}
		elevSetStopLamp(0);

		while(!hwInterfaceCheckOrders()) { // brake if new order
			if (elevGetStopSignal())
			{
				elevSetStopLamp(1);
				while(elevGetStopSignal()) {// Allows pressing buttons even elev is polling in indefined state.
					continue;
				}
				elevSetStopLamp(0);
			}
			continue;
		}

		//Følgende algoritme avgjør om heisen skal opp eller ned hvis heisen bestilles til samme etasje som etasje-indikatoren tilsier. Bruker da forrige retning til å bestemme om den skal opp eller ned
		//Desides if elev is going up or down if order is same as floor indicator, based on current and last_dir
		if (hwInterfaceGetNextFloor() == hwInterfaceGetCurrentFloor())
		{
			if(last_dir == DIRN_UP){
				if (hwInterfaceGetCurrentFloor() != 3){
					hwInterfaceSetCurrentFloor(hwInterfaceGetCurrentFloor() + 1);
				}
			}
			else if (last_dir == DIRN_DOWN)
			{
				if(hwInterfaceGetCurrentFloor() != 0) {
					hwInterfaceSetCurrentFloor(hwInterfaceGetCurrentFloor() - 1);
				}
			}		
		}
		
	}
	
}
