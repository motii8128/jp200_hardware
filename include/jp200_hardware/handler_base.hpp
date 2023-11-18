#ifndef HANDLER_BASE_HPP_
#define HANDLER_BASE_HPP_

#include <string>

namespace jp200_hardware
{
    class HandlerBase
        {
            public:
                static const int default_baudrate = 115200;
                
                static HandlerBase *getHandler(const std::string port_name);

                bool is_using_;

                virtual ~HandlerBase() {};

                virtual bool openPort() = 0;

                virtual void closePort() = 0;

                virtual void clearPort() = 0;

                virtual void setPortName(const std::string port_name) = 0;

                virtual std::string getPortName() = 0;

                virtual bool setBaudrate(const int baudrate) = 0;

                virtual int getBaudrate() = 0;

                virtual int getBytesAvailable() = 0;

                virtual int readPort(uint8_t *buffer, int length) = 0;

                virtual int writePort(uint16_t *buffer, int length) = 0;

                virtual void setPacketTimeout(uint16_t packet_length) = 0;
                
                virtual void setPacketTimeout(double msec) = 0;

                virtual bool isPacketTimeout() = 0;
        };
}

#endif