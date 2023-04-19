#ifndef UDS_UDSSERVER_H
#define UDS_UDSSERVER_H

#include <unordered_map>
#include <chrono>
#include <windows.h>
#include "PCANBasic.h"
#include "Uds.h"

class UdsServer {
public:
    UdsServer();
    void handleRequest();
    void checkTesterPresent();

    void initialize();
    void sendResponse(unsigned int response, uint8_t dataFrame[]);

private:
    UDSsession currentSession;
    UDSsession requestedSessionCode = 0x01;
    bool testerPresent;
    std::chrono::time_point<std::chrono::steady_clock> lastTesterPresentTime;
    std::unordered_map<int, void (UdsServer::*)()> serviceHandlers;
    TPCANMsg CANMsg{};
    TPCANStatus status{};
    DWORD dwResult{};

    void handleSessionControl();
    void handleReadData();
    void handleTesterPresent();
};
#endif //UDS_UDSSERVER_H
