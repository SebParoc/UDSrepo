#include "../HeaderFiles/UdsClient.h"
#include "../HeaderFiles/UdsServer.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include "vxlapi.h"

UdsClient::UdsClient(char *name, XLportHandle portHandle, XLaccess accessMask)
        : name(name), portHandle(portHandle), accessMask(accessMask) {
    initialize();
}

void UdsClient::sendRequest(std::string requestMessage) {

    UdsMessage * message;
    int count = 1;
    for (char c : requestMessage) {
        if (c == ' ') count++;
    }
    //status = xlCanFlushTransmitQueue(portHandle, accessMask);
    XLevent event;
    unsigned int nCount = 1;

    event.tag = XL_TRANSMIT_MSG;
    event.tagData.msg.id = XL_CAN_EXT_MSG_ID | 0x18DAFF00;
    if (count < 8) {
        message = new SFmessage;
    }
    else {
        message = new FFmessage;
    }
    uint8_t *temp = message->processInput(requestMessage);
    uint8_t dataFrame[8];
    std::memcpy(dataFrame, temp, 8);

    event.tagData.msg.dlc = 8;
    event.tagData.msg.data[0] = static_cast<int>(dataFrame[0]);
    event.tagData.msg.data[1] = static_cast<int>(dataFrame[1]);
    event.tagData.msg.data[2] = static_cast<int>(dataFrame[2]);
    event.tagData.msg.data[3] = static_cast<int>(dataFrame[3]);
    event.tagData.msg.data[4] = static_cast<int>(dataFrame[4]);
    event.tagData.msg.data[5] = static_cast<int>(dataFrame[5]);
    event.tagData.msg.data[6] = static_cast<int>(dataFrame[6]);
    event.tagData.msg.data[7] = static_cast<int>(dataFrame[7]);

    status = xlCanTransmit(portHandle, accessMask, &nCount, &event);

    if (status != XL_SUCCESS)
        std::cerr << "Error transmitting: " << status << ". " << xlGetErrorString(status) << std::endl;

    std::thread t(&UdsClient::handleResponse, this);
    t.join();
    }

void UdsClient::initialize() {

    XLaccess permissionMask = accessMask;
    status = xlOpenDriver();
    if (status != XL_SUCCESS) {
        std::cerr << "Failed to open client driver";
    }

    status = xlOpenPort(&portHandle, name, accessMask, &permissionMask, 1024, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
    if (status != XL_SUCCESS) {
        std::cerr << "Failed to open client port. Error code: " << status << std::endl;
        xlCloseDriver();
    }

    status = xlSetTimerRate(portHandle, 100);
    status = xlActivateChannel(portHandle, accessMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
    if (status != XL_SUCCESS) {
        std::cerr << "Failed to activate channel (client). Error code: " << status << std::endl;
        xlClosePort(portHandle);
        xlCloseDriver();
    }
}

void UdsClient::handleResponse() {
    HANDLE h;
    unsigned int msgsrx;
    XLevent event;
    auto start = std::chrono::high_resolution_clock::now();
        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < 10) {
            msgsrx = 1;
            h = CreateEvent(NULL, FALSE, FALSE, NULL);
            status = xlSetNotification(portHandle, &h,  1);
            while(WaitForSingleObject(h, 10000) == WAIT_TIMEOUT);
            do {
            //std::cout << "xd";
                    status = xlReceive(portHandle, &msgsrx, &event);
                    if(event.tag == XL_RECEIVE_MSG && event.tagData.msg.id) {
                        std::cout << "tag" << std::hex << static_cast<int>(event.tag) << " Received response: [";
                        for(int i = 0; i < 8; i++) {
                            std::cout << std::hex << std::uppercase << static_cast<int>(event.tagData.msg.data[i]) << ' ';
                        }
                        std::cout << "]";
                        if(event.tagData.msg.data[1] == DIAGNOSTIC_SESSION_CONTROL_SUCCESS) {
                            currentSession = event.tagData.msg.data[2];
                        }
                        //return;
                    }
                } while(status == 0);
        }
    }
