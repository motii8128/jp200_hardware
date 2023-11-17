#ifndef PORT_HANDLER_HPP_
#define PORT_HANDLER_HPP_

#include <string>
#include "jp200_hardware/port_handler_base.hpp"

namespace jp200_hardware
{
    class PortHandler : public PortHandlerBase
    {
        private:
            int socket_fd_;
            int baudrate_;
            std::string port_name_;

            double packet_start_time_;
            double packet_timeout_;
            double tx_time_per_bytes;

            bool setupPort(const int c_flag_baud);
            bool setCustomBaudrate(int speed);
            int getcFlagBaud(const int baudrate);

            double getCurrentTime();
            double getTimeSinceStart();

        public:
            
            PortHandler(const std::string port_name);

            virtual ~PortHandler() {closePort();};

            bool openPort();

            void closePort();

            void clearPort();

            void setPortName(const std::string port_name);

            std::string getPortName();

            bool setBaudrate(const int baudrate);

            int getBaudrate();

            int getBytesAvailable();

            int readPort(uint8_t *buffer, int length);

            int writePort(uint16_t *buffer, int length);

            void setPacketTimeout(uint16_t packet_length);
            
            void setPacketTimeout(double msec);

            bool isPacketTimeout();
    };
} // namespace jp200_hardware

#endif 