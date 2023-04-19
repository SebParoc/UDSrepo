//
// Created by spazr on 4/14/2023.
//

#include "Handler.h"
#include <thread>
#include <chrono>
#include <iostream>

void Handler::setClient(UdsClient _client)  {
    client = _client;
}

void Handler::handleRequest(int number) {
    std::cout << "Requested number: " << number << std::endl;
}

void Handler::handleRequestSF(const char *input) {
    /*(auto * message = new SFmessage;
    message->processInput(number);
    delete message;*/
    client.sendRequest(input);
}

void Handler::handleRequestFF(const char *input) {
    auto * message = new FFmessage;
    message->processInput(input);
    delete message;
}

void Handler::goToSleep(int number) {
    std::this_thread::sleep_for(std::chrono::milliseconds(number));
}

