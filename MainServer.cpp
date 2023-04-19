#include "HeaderFiles/UdsServer.h"
#include <iostream>

using namespace std;

int main() {
    UdsServer server;
    //server.initialize();
    //server.handleRequest();
    uint8_t dataFrame[8] = {0x02, 0x10, 0x01, 0xff, 0xff, 0xff, 0xff};
    server.sendResponse(0x50, dataFrame);
}