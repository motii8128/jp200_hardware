#include "jp200_hardware/packet_handler.hpp"

#include <string>

#define TXPACKET_MAX_LEN    (250)
#define RXPACKET_MAX_LEN    (250)

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

    int PacketHandler::TxPacket(HandlerBase *port, uint8_t *tx_packet)
    {
        uint8_t checksum = 0;
        uint8_t total_packet_length = tx_packet[PKT_LENGTH] + 4;
        uint8_t written_packet_length = 0;

        if(port->is_using_)
            return COMM_PORT_BUSY;
        port->is_using_ = true;

        if(total_packet_length > TXPACKET_MAX_LEN)
        {
            port->is_using_ = false;
            return COMM_TX_ERROR;
        }

        tx_packet[PKT_HEADER0] = 0xff;
        tx_packet[PKT_HEADER1] = 0xff;

        for(uint16_t idx = 2; idx < total_packet_length; idx++)
            checksum += tx_packet[idx];
        tx_packet[total_packet_length - 1] = ~checksum;

        port->clearPort();
        written_packet_length = port->writePort(tx_packet);
    }
}