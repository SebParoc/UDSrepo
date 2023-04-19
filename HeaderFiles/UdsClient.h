#ifndef UDS_UDSCLIENT_H
#define UDS_UDSCLIENT_H
#include "vxlapi.h"
#include "Uds.h"

class UdsClient {
public:
    UdsClient() = default;
    UdsClient(char *name, XLportHandle portHandle, XLaccess accessMask);

    void sendRequest(std::string requestMessage);
    void handleResponse();

private:
    UDSsession currentSession;
    XLportHandle portHandle;
    char *name;
    XLaccess accessMask;
    XLstatus status{};

    void initialize();

};

#endif //UDS_UDSCLIENT_H
