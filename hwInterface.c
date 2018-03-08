#include "liftCtrl.h"
#include "hwInterface.h"
#include "elev.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

//Private variable

int current_floor = 0;
int next_floor = 0;
int current_dir = 0;
int last_dir = 0;

int order_table[3][4] ={
// 	1 	2 	3 	4 (Etasjenummer)
	{0, 0, 0, 0},	//Orders going up
	{0, 0, 0, 0},	//Orders going down
	{0, 0, 0, 0}	//Panel
};

void hwInterfaceResetOrders() {

	for (int floor = 0; floor < 4; floor++)
	{
		for (int button = 0; button < 3; button++)
		{
			if (floor == 0 && button == BUTTON_CALL_DOWN) continue;
			if (floor == N_FLOORS-1 && button == BUTTON_CALL_UP) continue;
			order_table[button][floor] = 0;
			elevSetButtonLamp(button,floor,0);
		}
	}

}

int hwInterfaceCheckOrders() {

	//iterate the button signals, and returns if a order button is pressed

	for(int floor = 0; floor < 4; floor++) { 									
		for (int button = 0; button < 3; button++) {
			if (floor == 0 && button == BUTTON_CALL_DOWN) continue;
			if (floor == N_FLOORS-1 && button == BUTTON_CALL_UP) continue;
			if (elevGetButtonSignal(button,floor)) {

				if (order_table[button][floor] == 0){
					order_table[button][floor] = 1;
					elevSetButtonLamp(button,floor,1); 	

					if (hwInterfaceGetNumberOfOrders() == 1){
					hwInterfaceSetNextFloor(floor);
					}
					return 1;
				}

				else{
					elevSetButtonLamp(button,floor,1); 	

					if (hwInterfaceGetNumberOfOrders() == 1){
					hwInterfaceSetNextFloor(floor);
					continue;
					}
				}
			}
		}
	}
	return 0;
}

int hwInterfaceGetDownstairsOrder() {

	for (int floor = current_floor; floor >= 0; floor--){ //Checks for down buttons and cab buttons below the current floor of the elevator. 
		if (order_table[1][floor] || order_table[2][floor]){
			next_floor = floor;
			return 1;
		}
	}

	for (int floor = 0; floor <= 3; floor++){
		if((order_table[1][floor] == 1 || order_table[0][floor] == 1 || order_table[2][floor])){ //Cheks the rest of the buttons on the panel

			next_floor = floor;
			return 1;
		}
	}
	return 0;
}

int hwInterfaceGetUpstairsOrder() {

	for (int floor = current_floor; floor < 4; floor++){ //Checks for up buttons and cab buttons above the current floor of the elevator.
		if (order_table[0][floor] || order_table[2][floor] ){
			next_floor = floor;
			return 1;
		}
	}

	for (int floor = 3; floor >= 0; floor--){
		if((order_table[1][floor] == 1 || order_table[0][floor] == 1 || order_table[2][floor])){ //Cheks the rest of the buttons on the panel
			next_floor = floor;
			return 1;
		}
	}
	return 0;
}


void hwInterfaceDeleteAllOrders() {
	//Deletes all orders on the current floor
	for (int button = 0; button < 3; button++){
		order_table[button][current_floor] = 0;
		if (current_floor == 0 && button == BUTTON_CALL_DOWN) continue;
		if (current_floor == N_FLOORS-1 && button == BUTTON_CALL_UP) continue;
		elevSetButtonLamp(button, current_floor, 0);
	}
}


void hwInterfaceSetNextFloorPriority() {
	// Desides priority on next floor based on the current and last direction of the elevator. 
	if (current_dir == DIRECTION_UP){
		hwInterfaceGetUpstairsOrder();
		return;
	} else if(current_dir == DIRECTION_DOWN) {
		hwInterfaceGetDownstairsOrder();
		return;
	} else if(current_dir == 0 && last_dir == DIRECTION_UP && current_floor != 3) {
		hwInterfaceGetUpstairsOrder();
		return;
	} else if(current_dir == 0 && last_dir == DIRECTION_DOWN && current_floor != 0) {
		hwInterfaceGetDownstairsOrder();
		return;
	} else if(current_dir == 0 && last_dir == DIRECTION_UP && current_floor == 3) {
		hwInterfaceGetDownstairsOrder();
		return;
	} else if(current_dir == 0 && last_dir == DIRECTION_DOWN && current_floor == 0) {
		hwInterfaceGetUpstairsOrder();
		return;
	}

}

int hwInterfaceGetNumberOfOrders() {
	int sum = 0;
	for (int floor = 0; floor < 4; floor++) {
		for(int button = 0; button < 3; button++) {
			sum += order_table[button][floor];
		}
	}
	return sum;
}

void hwInterfaceSetNextFloor(int floor) {
	assert(floor>=0);
	next_floor = floor;
}

int hwInterfaceGetNextFloor() {
	assert(next_floor>=0);
	return next_floor;
}

void hwInterfaceSetCurrentFloor(int floor) {
	current_floor = floor;
}

int hwInterfaceGetCurrentFloor() {
	return current_floor;
}

void printNextFloor() {
	printf("next floor: %d\n",next_floor);
}

void printCurrentFloor() {
	printf("current floor: %d\n",current_floor);
}

void printMatrix(){

	for(int floor = 0; floor < 4; floor++) {									
		for (int button = 0; button < 3; button++) {
			printf("%d, ", order_table[button][floor]);

				if (button == 2) {
					printf("\n");
				}
		}
	}
}

void hwInterfaceSetCurrentDir(int direction){
	current_dir = direction;
}


int hwInterfaceGetCurrentDir() {
	return current_dir;
}

int hwInterfaceGetLastDir() {
	return last_dir;
}

void hwInterfaceSetLastDir(int direction) {
	last_dir = direction;
}
