#include "jp200_hardware/packet_handler.hpp"

#include <string>

namespace jp200_hardware
{
    PacketHandler *PacketHandler::unique_instance_ = new PacketHandler();

    PacketHandler::PacketHandler(){}

    const std::string PacketHandler::getTxRxResult(int result)
    {
        switch (result)
        {
        case COMM_SUCCESS:
            return "[TxRxResult] Communication success.";

        case COMM_PORT_BUSY:
            return "[TxRxResult] Port is in use!!";

        case COMM_TX_FAIL:
            return "[TxRxResult] Failed transmit cmd packet!!";

        case COMM_RX_FAIL:
            return "[TxRxResult] Failed get status packet from SERVO!!";

        case COMM_TX_ERROR:
            return "[TxRxResult] Incorrect cmd packet!!";

        case COMM_RX_WAITING:
            return "[TxRxResult] Now recieving status from SERVO!!";

        case COMM_RX_TIMEOUT:
            return "[TxRxResult] There is no status packet!!";

        case COMM_RX_CORRUPT:
            return "[TxRxResult] Incorrect cmd packet!!";

        case COMM_NOT_AVAILABLE:
            return "[TxRxResult] Can't open serial port!!";

        
        default:
            return "";
        }
    }
}