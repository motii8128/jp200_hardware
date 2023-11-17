#include "jp200_hardware/port_handler.hpp"
#include "jp200_hardware/port_handler_base.hpp"
#include <string>

using namespace jp200_hardware;

PortHandlerBase *PortHandlerBase::getPortHandler(const std::string port_name)
{
    return (PortHandler *)(new PortHandler(port_name));
}