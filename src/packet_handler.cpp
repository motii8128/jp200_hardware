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
        written_packet_length = port->writePort(tx_packet, total_packet_length);

        if(total_packet_length != written_packet_length)
        {
            port->is_using_ = false;
            return COMM_TX_FAIL;
        }

        return COMM_SUCCESS;
    }

    int PacketHandler::RxPacket(HandlerBase *port, uint8_t *rx_packet, bool skip_stuffing)
    {
        int result = COMM_TX_FAIL;

        uint8_t checksum = 0;
        uint8_t rx_length = 0;
        uint8_t wait_length = 6;

        while(true)
        {
            rx_length += port->readPort(&rx_packet[rx_length], wait_length - rx_length);
            if(rx_length >= wait_length)
            {
                uint8_t idx = 0;

                for(idx = 0; idx < (rx_length - 1); idx++)
                {
                    if(rx_packet[idx] == 0xff && rx_packet[idx+1] == 0xff)
                    {
                        break;
                    }
                }

                if(idx == 0)
                {
                    // error check
                    if(rx_packet[PKT_ID] > 0xFD || rx_packet[PKT_LENGTH] > RXPACKET_MAX_LEN || rx_packet[PKT_ERROR] > 0x7F)
                    {
                        for(uint16_t s = 0; s < rx_length-1; s++)
                        {
                            rx_packet[s] = rx_packet[s + 1];
                        }
                        rx_length -= 1;
                        continue;
                    }

                    if(wait_length != rx_packet[PKT_LENGTH] + PKT_LENGTH + 1)
                    {
                        wait_length = rx_packet[PKT_LENGTH] + PKT_LENGTH + 1;
                        continue;
                    }

                    if(rx_length < wait_length)
                    {
                        if(port->isPacketTimeout() == true)
                        {
                            if(rx_length == 0)
                            {
                                result = COMM_RX_TIMEOUT;
                            }
                            else
                            {
                                result = COMM_RX_CORRUPT;
                            }
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    for(uint16_t i = 2; i < wait_length - 1; i++)
                    {
                        checksum += rx_packet[i];
                    }

                    checksum = ~checksum;

                    if(rx_packet[wait_length - 1] == checksum)
                    {
                        result = COMM_SUCCESS;
                    }
                    else
                    {
                        result = COMM_RX_CORRUPT;
                    }
                    break;
                }
                else
                {
                    for(uint16_t s = 0; s < rx_length; s++)
                    {
                        rx_packet[s] = rx_packet[idx + s];
                    }
                    rx_length -= idx;
                }
            }
            else
            {
                if(port->isPacketTimeout() == true)
                {
                    if(rx_length == 0)
                    {
                        result = COMM_RX_TIMEOUT;
                    }
                    else
                    {
                        result = COMM_RX_CORRUPT;
                    }
                    break;
                }
            }
        }
        port->is_using_ = false;

        return result;
    }

    int PacketHandler::TxRxPacket(HandlerBase *port, uint8_t *tx_packet, uint8_t *rx_packet, uint8_t *err)
    {
        int result = COMM_TX_FAIL;

        result = TxPacket(port, tx_packet);
        if(result != COMM_SUCCESS)
        {
            return result;
        }

        if (tx_packet[PKT_ID] == BROADCAST_ID)
        {
            port->is_using_ = false;
            return result;
        }

        // set packet timeout
        if (tx_packet[PKT_INSTRUCTION] == 2)
        {
            port->setPacketTimeout((uint16_t)(tx_packet[PKT_PARAMETER0+1] + 6));
        }
        else
        {
            port->setPacketTimeout((uint16_t)6);
        }

        // rx packet
        do {
            result = RxPacket(port, rx_packet, false);
        } while (result == COMM_SUCCESS && tx_packet[PKT_ID] != rx_packet[PKT_ID]);

        if (result == COMM_SUCCESS && tx_packet[PKT_ID] == rx_packet[PKT_ID])
        {
            if (err != 0)
            *err = (uint8_t)rx_packet[PKT_ERROR];
        }

        return result;
    }

    int PacketHandler::ID(HandlerBase *port, uint8_t id, uint8_t *err)
    {
        return ID(port, id, 0, err);
    }

    int PacketHandler::ID(HandlerBase *port, uint8_t id, uint16_t *model_number, uint8_t *err)
    {
        int result = COMM_TX_FAIL;

        uint8_t tx_packet[6] = {0};
        uint8_t rx_packet[6] = {0};

        if(id >= BROADCAST_ID)
        {
            return COMM_NOT_AVAILABLE;
        }

        tx_packet[PKT_ID] = id;
        

        return result;
    }

    int PacketHandler::broadcastID(HandlerBase *port, std::vector<uint8_t> &id_list)
    {
        return COMM_NOT_AVAILABLE;
    }

    int PacketHandler::action(HandlerBase *port, uint8_t id)
    {
        uint8_t tx_packet[6] = {0};

        tx_packet[PKT_ID] = id;
        tx_packet[PKT_LENGTH] = 2;
        tx_packet[PKT_INSTRUCTION] = 5;

        //return TxRxPacket(port, tx_packet, 0);
    }

    int PacketHandler::reboot(HandlerBase *port, uint8_t id, uint8_t *err)
    {
        return COMM_NOT_AVAILABLE;
    }

    int PacketHandler::clearMultiTurn(HandlerBase *port, uint8_t id, uint8_t *err)
    {
        return COMM_NOT_AVAILABLE;
    }

    int PacketHandler::factoryReset(HandlerBase *port, uint8_t id, uint8_t option, uint8_t *err)
    {
        uint8_t tx_packet[6] = {0};
        uint8_t rx_packet[6] = {0};

        tx_packet[PKT_ID] = id;
        tx_packet[PKT_LENGTH] = 2;
        tx_packet[PKT_INSTRUCTION] = 4;

        return TxRxPacket(port, tx_packet, rx_packet, err);
    }

    int PacketHandler::readTx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length)
    {
        int result = COMM_TX_FAIL;

        uint8_t tx_packet = {0};

        if(id >= BROADCAST_ID)
        {
            return COMM_NOT_AVAILABLE;
        }

        
    }
}