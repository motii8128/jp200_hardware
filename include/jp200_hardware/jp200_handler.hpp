#ifndef JP200_HANDLER_HPP_
#define JP200_HANDLER_HPP_

#include <string>

class JP200Handler
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
        
        static JP200Handler getPortHandler(const std::string port_name);

        bool is_using_;

        virtual ~JP200Handler(){closePort();};

        bool openPort();

        void closePort();

        void clearPort();

        void setPortName(const std::string port_name);

        std::string *getPortName();

        bool setBaudrate(const int baudrate);

        int getBaudrate();

        int getBytesAvailable();

        int readPort(uint8_t *packet, int length);

        int writePort(uint8_t *packet, int length);

        void setPacketTimeout(uint16_t packet_length);
        
        void setPacketTimeout(double msec);

        bool isPacketTimeout();
};

#endif 