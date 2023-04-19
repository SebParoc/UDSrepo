//
// Created by spazr on 4/14/2023.
//

#ifndef UDS_HANDLER_H
#define UDS_HANDLER_H

#pragma once

#include "../HeaderFiles/Uds.h"
#include "../HeaderFiles/UdsClient.h"
#include "../HeaderFiles/UdsServer.h"

class Handler{
public:
    Handler() = default;

    void setClient(UdsClient _client);

    void handleRequest(int number);

    void handleRequestSF(const char * input);

    void handleRequestFF(const char * input);

    void goToSleep(int number);
private:
    UdsClient client;
};

#endif //UDS_HANDLER_H
