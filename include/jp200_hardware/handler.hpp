#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <string>

class Handler
{
    private:
        int socket_fd_;
        int boudrate_;
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
        static const int default_baudrate = 115200;
        
        Handler(const std::string *port_name);
        
};

#endif 