//Priority functions

void hwInterfaceResetAllOrders(); 
int hwInterfaceCheckOrders(); 
void hwInterfaceSetNextFloorPriority();
void hwInterfaceDeleteOrders(); 
int hwInterfaceGetUpstairsOrder(); 
int hwInterfaceGetDownstairsOrder();
int hwInterfaceGetNumberOfOrders();

// Getters and setters
void hwInterfaceSetNextFloor(int floor);
int hwInterfaceGetNextFloor();
void hwInterfaceSetCurrentFloor(int floor);
int hwInterfaceGetCurrentFloor();
void hwInterfaceSetCurrentDir(int direction);
int hwInterfaceGetCurrentDir();
void hwInterfaceSetLastDir(int direction);
int hwInterfaceGetLastDir();

// Utilities
void printNextFloor();
void printCurrentFloor();
void printMatrix();