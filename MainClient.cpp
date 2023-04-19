#include "vxlapi.h"
#include <iostream>
#include <thread>
#include <string>
#include "HeaderFiles/UdsClient.h"
#include "HeaderFiles/Uds.h"
#include "ScriptReader/Handler.h"
#include "ScriptReader/parser.tab.h"

extern int parser_main(int argc, char **argv, Handler _handler);

int main(int argc, char **argv) {
    Handler handler;
    XLportHandle portHandle = XL_INVALID_PORTHANDLE;
    XLaccess clientAccessMask = 1;
    UdsClient client((char *) "name", portHandle, clientAccessMask);
    handler.setClient(client);
    //std::thread receiveResponses([&client](){client.handleResponse();});
    //receiveResponses.join();
    client.sendRequest("\"10 01\"");
    system("pause");
    //parser_main(argc, argv, handler);
    //std::thread scriptReaderThread(parser_main, argc, argv, handler);
    std::cout << "Out of parser\n";
    /*scriptReaderThread.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::string reqId;
    while(true) {
        getline(std::cin, reqId);
        client.sendRequest(const_cast<char *>(reqId.c_str()));
    }*/
}