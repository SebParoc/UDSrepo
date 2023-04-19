#include "../HeaderFiles/UdsServer.h"
#include <iostream>
#include <thread>

using namespace std;

UdsServer::UdsServer(): currentSession(0x00), testerPresent(false), lastTesterPresentTime(std::chrono::steady_clock::now()){
    serviceHandlers = {
            {0x10, &UdsServer::handleSessionControl},
            {0x22, &UdsServer::handleReadData},
            {0x3E, &UdsServer::handleTesterPresent},
    };
}

void UdsServer::initialize() {
    status = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_500K, 0, 0, 0);
    if (status != PCAN_ERROR_OK) {
        cerr << "Error initializing the PCAN Channel" << endl;
    }
}

void UdsServer::handleRequest() {

    const int TP_INTERVAL_MS = 1000; // 1 second
    bool sessionActive = false;
    time_t lastRequestTime = 0;
    time_t lastTpTime = 0; 

    while (true) {

        if (currentSession != DEFAULT_SESSION) {
            checkTesterPresent();
        }
        status = CAN_Read(PCAN_USBBUS1, &CANMsg, nullptr);
        if (status == PCAN_ERROR_OK) {
            cout << "Received message: ID= " << hex << CANMsg.ID << " DATA=";

            int num_ff = 0;
            for (int i = 0; i < CANMsg.LEN; i++) {
                if ((int) CANMsg.DATA[i] == 0xff)
                    num_ff++;
                cout << hex << (int) CANMsg.DATA[i] << ' ';
            }
            int nBytes = (int) CANMsg.DATA[0];
            if (nBytes == 0xff || nBytes != CANMsg.LEN - num_ff - 1) {
                cout << "Invalid request" << endl;
                sendResponse(NEGATIVE_RESPONSE, CANMsg.DATA);

            } else {
                auto it = serviceHandlers.find((int) CANMsg.DATA[1]);
                if (it != serviceHandlers.end()) {
                    if (it == serviceHandlers.find(0x10)) {
                        requestedSessionCode = (int) CANMsg.DATA[2];
                    }
                    (this->*it->second)();
                } else {
                    cout << "Service not supported" << endl;
                    sendResponse(NEGATIVE_RESPONSE, CANMsg.DATA);
                }
            }
        } else if (status != PCAN_ERROR_QRCVEMPTY) {
            cout << "Error reading from the PCAN Channel" << std::endl;
            break;
        }

        Sleep(10);
    }

}

void UdsServer::checkTesterPresent() {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::seconds>(now - lastTesterPresentTime).count();
    if (elapsed > 5) {
        cout << "Tester not present" << endl;
        currentSession = DEFAULT_SESSION;
        testerPresent = false;
    }
}

void UdsServer::handleSessionControl() {
    switch (requestedSessionCode) {
        case DEFAULT_SESSION:
        case EXTENDED_DIAGNOSTIC_SESSION:
        case PROGRAMMING_SESSION:
            cout << "Switched session. Session code:  " << requestedSessionCode << "\n" << endl;
            currentSession = requestedSessionCode;
            sendResponse(DIAGNOSTIC_SESSION_CONTROL_SUCCESS, CANMsg.DATA);
            break;
        default:
            cout << "invalid session code: " << requestedSessionCode << endl;
            currentSession = DEFAULT_SESSION;
            sendResponse(NEGATIVE_RESPONSE, CANMsg.DATA);
            break;
    }
}

void UdsServer::handleReadData() {
    if (currentSession == DEFAULT_SESSION) {
        cout << "Reading data" << std::endl;
    } else {
        cout << "Service not supported in current session" << endl;
    }
}

void UdsServer::handleTesterPresent() {
    cout << "Tester present" << endl;
    testerPresent = true;
    sendResponse(TESTER_PRESENT_SUCCESS, CANMsg.DATA);
    lastTesterPresentTime = chrono::steady_clock::now();
}

void UdsServer::sendResponse(unsigned int response, uint8_t * dataFrame) {
    TPCANMsg responseMsg;
    responseMsg.ID = 0x12345678;
    responseMsg.MSGTYPE = PCAN_MESSAGE_EXTENDED;
    responseMsg.LEN = 3;
    for(int i=0; i<8; i++) {
        std::cout << std::hex << static_cast<int>(dataFrame[i]) << "; ";
    }
    if(response == NEGATIVE_RESPONSE) {
        responseMsg.DATA[0] = response;
        responseMsg.DATA[1] = dataFrame[1];
        responseMsg.DATA[2] = 0xff;
        responseMsg.DATA[3] = 0xff;
        responseMsg.DATA[4] = 0xff;
        responseMsg.DATA[5] = 0xff;
        responseMsg.DATA[6] = 0xff;
        responseMsg.DATA[7] = 0xff;
    }
    else {
        responseMsg.DATA[0] = 0x29;
        responseMsg.DATA[1] = response;
        responseMsg.DATA[2] = dataFrame[2];
        responseMsg.DATA[3] = dataFrame[3];
        responseMsg.DATA[4] = 0xff;
        responseMsg.DATA[5] = 0xff;
        responseMsg.DATA[6] = 0xff;
        responseMsg.DATA[7] = 0xff;
    }

    status = CAN_Write(PCAN_USBBUS1, &responseMsg);
    if (status != PCAN_ERROR_OK) {
        char * strMsg = new char[256];
        CAN_GetErrorText(status, 0, strMsg);
        cerr << "Error: " << strMsg << endl;
        delete[] strMsg;
    } else {
        cout << "Response " << hex << response << endl;
    }
}


