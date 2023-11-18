#include "jp200_hardware/packet_handler.hpp"

#include <string>

namespace jp200_hardware
{
    PacketHandler *PacketHandler::unique_instance_ = new PacketHandler();

    PacketHandler::PacketHandler(){}

    std::string PacketHandler::getTxRxResult(int result)
    {
        switch (result)
        {
        case COMM_SUCCESS:
            /* code */
            break;
        
        default:
            break;
        }
    }
}