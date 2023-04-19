#ifndef UDS_UDS_H
#define UDS_UDS_H

#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

/******************************************************
 * Sessions
 ******************************************************/
typedef uint8_t UDSsession;

#define DEFAULT_SESSION                         0x01
#define PROGRAMMING_SESSION                     0x02
#define EXTENDED_DIAGNOSTIC_SESSION             0x03

/******************************************************
 * Services Response
 ******************************************************/
#define NEGATIVE_RESPONSE                       0x7F

#define DIAGNOSTIC_SESSION_CONTROL_SUCCESS      0x50
#define SECURITY_ACCESS_SUCCESS                 0x67
#define TESTER_PRESENT_SUCCESS                  0x7E

/*******************************************************
 *
 *******************************************************/

#define S3_TIMER_CLIENT                         4000
#define S3_TIMER_SERVER                         5000

/*******************************************************
 * Messages
 ******************************************************/

class UdsMessage {
public:
    UdsMessage() {}
    virtual ~UdsMessage() = default;
    std::uint8_t sid;
    std::uint16_t did;
    const std::uint8_t* data;
    std::uint16_t length;
    std::uint8_t subfunction = 0;

    virtual std::uint8_t* processInput(std::string input) = 0;
    virtual std::string getMessageType() = 0;
};

class SFmessage : public UdsMessage {
public:
    std::uint8_t pci;

    SFmessage() : UdsMessage() {}
    ~SFmessage() {}

    std::uint8_t* processInput(std::string input) override {
        std::uint8_t dataFrame[8];
        input = input.substr(1, input.length() - 2);
        std::istringstream iss(input);
        std::string byteString;
        std::vector<std::uint8_t> bytes;
        while (std::getline(iss, byteString, ' ')) {
            bytes.push_back(std::stoul(byteString, nullptr, 16));
        }
        while (bytes.size() < 7) {
            bytes.push_back(0xFF);
        }
        std::size_t count = std::count(bytes.begin(), bytes.end(), 0xFF);
        sid = bytes[0];
        if(sid == 0x10) {
            subfunction = bytes[1];
            did = (bytes[2] << 8) | bytes[3];
            data = bytes.data() + 4;
            length = bytes.size() - count;
            pci = bytes.size() - count;
        }
        else {
            did = (bytes[1] << 8) | bytes[2];
            data = bytes.data() + 3;
            length = bytes.size() - (3 + count);
            pci = bytes.size() - count;
        }

        int i = 0;
        dataFrame[i++] = static_cast<int>(pci);
        std::cout << std::hex << "pci: 0x" << std::setfill('0') <<  static_cast<int>(pci) << std::endl;
        dataFrame[i++] = static_cast<int>(sid);
        std::cout << "sid: 0x" << static_cast<int>(sid) << std::endl;
        if(subfunction != 0) {
            std::cout << "subfunction: 0x" << static_cast<int>(subfunction) << std::endl;
            dataFrame[i++] = static_cast<int>(subfunction);
        }
        dataFrame[i++] = (did >> 8) & 0xFF;
        dataFrame[i++] = did & 0xFF;
        std::cout << "did: 0x" << did << std::endl;
        std::cout << "data: ";
        int j = i++;
        for (int k = 0; k < length; k++) {
            dataFrame[j++] = static_cast<int>(data[k]);
            std::cout << "0x" << std::setfill('0') << static_cast<int>(data[k]) << " ";
        }
        for (j; j < 8; j++) {
            dataFrame[j] = 0xFF;
        }
        std::cout << "[";
        for (unsigned char k : dataFrame) {
            std::cout << "0x" << std::hex << static_cast<int>(k) << " ";
        }
        std::cout << "]" << std::endl << std::endl;
        return dataFrame;
    }

    std::string getMessageType() override {
        return "Single Frame Message";
    }
};

class FFmessage : public UdsMessage {
public:
    std::uint16_t pci;

    std::uint8_t* processInput(std::string input) override {
        std::cout << "Implementation of FF coming soon..." << std::endl;
        return {};
    }

    std::string getMessageType() override {
        return "First Frame Message";
    }
};

class CFmessage : public UdsMessage {
public:
    std::uint8_t pci;

    std::uint8_t* processInput(std::string input) override {
        return {};
        //TODO: Implement the logic to extract and set the fields of a CF message from the input string
    }
};

#endif
