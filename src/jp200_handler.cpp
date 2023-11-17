#include "jp200_hardware/jp200_handler.hpp"
#include <unistd.h>

bool JP200Handler::openPort()
{
    return setBaudrate(boudrate_);
}

void JP200Handler::closePort()
{
    if(socket_fd_ != -1)
    {
        close(socket_fd_);
    }
    socket_fd_ = -1;
}
