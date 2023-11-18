#ifndef PACKET_HANDLER_HPP_
#define PACKET_HANDLER_HPP_

#include "jp200_hardware/handler_base.hpp"
#include <stdio.h>
#include <string>
#include <vector>

namespace jp200_hardware
{
    class PacketHandler
    {
        protected:
            PacketHandler() {}

        public:
            static PacketHandler *getPacketHandler(float protocol_ver = 2.0);

            virtual ~PacketHandler() {}

            virtual float getProtocolVersion() = 0;

            virtual const std::string getTxRxResult(int result) = 0;

            virtual const std::string getRxPacketError(uint8_t err) = 0;

            virtual int TxPacket(HandlerBase *port, uint8_t *tx_packet) = 0;

            virtual int RxPacket(HandlerBase *port, uint8_t *rx_packet) = 0;

            virtual int ID(HandlerBase *port, uint8_t id, uint8_t *err = 0) = 0;

            virtual int ID(HandlerBase *port, uint8_t id, uint16_t *model_number, uint8_t *err = 0) = 0;

            virtual int broadcastID(HandlerBase *port, std::vector<uint8_t> &id_list) = 0;
    };
}

#endif