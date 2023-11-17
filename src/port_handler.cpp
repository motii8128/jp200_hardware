#include "jp200_hardware/port_handler.hpp"
#include "rclcpp/rclcpp.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

namespace jp200_hardware
{
    PortHandler::PortHandler(const std::string port_name):
        socket_fd_(-1),
        baudrate_(default_baudrate),
        packet_start_time_(0.0),
        packet_timeout_(0.0),
        tx_time_per_bytes(0.0)
    {
        is_using_ = false;
        setPortName(port_name);
    }

    struct termios_jp200 {
    tcflag_t c_iflag;       
    tcflag_t c_oflag;       
    tcflag_t c_cflag;       
    tcflag_t c_lflag;       
    cc_t c_line;            
    cc_t c_cc[19];          
    speed_t c_ispeed;       
    speed_t c_ospeed;       
    };

    bool PortHandler::openPort()
    {
        return setBaudrate(baudrate_);
    }

    void PortHandler::closePort()
    {
        if(socket_fd_ != -1)
        {
            close(socket_fd_);
        }
        socket_fd_ = -1;
    }

    void PortHandler::clearPort()
    {
        tcflush(socket_fd_, TCIFLUSH);
    }

    void PortHandler::setPortName(const std::string port_name)
    {
        port_name_ = port_name;
    }

    std::string PortHandler::getPortName()
    {
        return port_name_;
    }

    bool PortHandler::setBaudrate(const int baudrate)
    {
        int baud = getcFlagBaud(baudrate);

        closePort();

        if(baud <= 0)
        {
            setupPort(B38400);
            baudrate_ = baudrate;
            return setCustomBaudrate(baudrate);
        }
        else
        {
            baudrate_ = baudrate;
            return setupPort(baud);
        }
    }

    int PortHandler::getBaudrate()
    {
        return baudrate_;
    }

    int PortHandler::getBytesAvailable()
    {
        int bytes_available;
        ioctl(socket_fd_, FIONREAD, &bytes_available);
        return bytes_available;
    }

    int PortHandler::readPort(uint8_t *buffer, int length)
    {
        return read(socket_fd_, buffer, length);
    }

    int PortHandler::writePort(uint16_t *buffer, int length)
    {
        return write(socket_fd_, buffer, length);
    }

    void PortHandler::setPacketTimeout(double msec)
    {
        packet_start_time_ = getCurrentTime();
        packet_timeout_ = msec;
    }

    bool PortHandler::isPacketTimeout()
    {
        if(getTimeSinceStart() > packet_timeout_)
        {
            packet_timeout_ = 0;
            return true;
        }
        else
        {
            return false;
        }
    }

    double PortHandler::getCurrentTime()
    {
        struct timespec tv;
        clock_gettime(CLOCK_REALTIME, &tv);

        return ((double)tv.tv_sec * 1000.0 + (double)tv.tv_nsec * 0.001 * 0.001);
    }

    double PortHandler::getTimeSinceStart()
    {
        double time;

        time = getCurrentTime() - packet_start_time_;

        if(time < 0.0)packet_start_time_ = getCurrentTime();

        return time;
    }

    bool PortHandler::setupPort(int c_flag_baud)
    {
        struct termios new_tio;

        socket_fd_ = open(port_name_.c_str(), O_RDWR|O_NOCTTY|O_NONBLOCK);

        if(socket_fd_ < 0)
        {
            RCLCPP_ERROR(rclcpp::get_logger("port_handler"), "Failed to open serial port!\n");
            return false;
        }

        bzero(&new_tio, sizeof(new_tio));

        new_tio.c_cflag = c_flag_baud | CS8 | CLOCAL | CREAD;
        new_tio.c_iflag = IGNPAR;
        new_tio.c_oflag      = 0;
        new_tio.c_lflag      = 0;
        new_tio.c_cc[VTIME]  = 0;
        new_tio.c_cc[VMIN]   = 0;

        tcflush(socket_fd_, TCIFLUSH);
        tcsetattr(socket_fd_, TCSANOW, &new_tio);

        tx_time_per_bytes = (1000.0 / (double)baudrate_) * 10.0;

        return true;
    }

    bool PortHandler::setCustomBaudrate(int speed)
    {
        struct termios_jp200 options;
    }

    int PortHandler::getcFlagBaud(const int baudrate)
    {
    switch(baudrate)
    {
        case 9600:
        return B9600;
        case 19200:
        return B19200;
        case 38400:
        return B38400;
        case 57600:
        return B57600;
        case 115200:
        return B115200;
        case 230400:
        return B230400;
        case 460800:
        return B460800;
        case 500000:
        return B500000;
        case 576000:
        return B576000;
        case 921600:
        return B921600;
        case 1000000:
        return B1000000;
        case 1152000:
        return B1152000;
        case 1500000:
        return B1500000;
        case 2000000:
        return B2000000;
        case 2500000:
        return B2500000;
        case 3000000:
        return B3000000;
        case 3500000:
        return B3500000;
        case 4000000:
        return B4000000;
        default:
        return -1;
    }
    }
} // namespace jp200_hardware