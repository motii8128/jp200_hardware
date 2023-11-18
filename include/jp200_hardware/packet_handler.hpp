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
        private:
            static PacketHandler *unique_instance_;

            PacketHandler();

        public:
            static PacketHandler *getInstance(){return unique_instance_;};

            virtual ~PacketHandler() { }

            const std::string getTxRxResult(int result);

            const std::string getRxPacketError(uint8_t err);

            int TxPacket(HandlerBase *port, uint8_t *tx_packet);

            int RxPacket(HandlerBase *port, uint8_t *rx_packet);

            int ID(HandlerBase *port, uint8_t id, uint8_t *err = 0);

            int ID(HandlerBase *port, uint8_t id, uint16_t *model_number, uint8_t *err = 0);

            int broadcastID(HandlerBase *port, std::vector<uint8_t> &id_list);

            int action(HandlerBase *port, uint8_t id);

            int reboot(HandlerBase *port, uint8_t id, uint8_t *err = 0);

            int clearMultiTurn(HandlerBase *port, uint8_t id, uint8_t *err = 0);

            int factoryReset(HandlerBase *port, uint8_t id, uint8_t option = 0, uint8_t *err = 0);

            int readTx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length);

            int readRx(HandlerBase *port, uint8_t id, uint16_t length, uint8_t *data, uint8_t *err = 0);

            int readTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length, uint8_t *data, uint8_t *err = 0);

            int read_1ByteTx(HandlerBase *port, uint8_t id, uint16_t addr);

            int read_1ByteRx(HandlerBase *port, uint8_t id, uint8_t *data, uint8_t *err = 0);

            int read_1ByteTxRx(HandlerBase *port, u_int8_t id, uint16_t addr, uint8_t *data, uint8_t *err = 0);

            int read_2ByteTx(HandlerBase *port, uint8_t id, uint16_t addr);

            int read_2ByteRx(HandlerBase *port, uint8_t id, uint8_t *data, uint8_t *err = 0);

            int read_2ByteTxRx(HandlerBase *port, u_int8_t id, uint16_t addr, uint8_t *data, uint8_t *err = 0);

            int read_4ByteTx(HandlerBase *port, uint8_t id, uint16_t addr);

            int read_4ByteRx(HandlerBase *port, uint8_t id, uint8_t *data, uint8_t *err = 0);

            int read_4ByteTxRx(HandlerBase *port, u_int8_t id, uint16_t addr, uint8_t *data, uint8_t *err = 0);

            int writeTxOnly(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length, u_int8_t *data);

            int writeTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length, uint8_t *data, uint8_t *err = 0);

            int write_1ByteTxOnly(HandlerBase *port, uint8_t id, uint16_t address, uint8_t data);

            int write_1ByteTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint8_t data, uint8_t *err = 0);

            int write_2ByteTxOnly(HandlerBase *port, uint8_t id, uint16_t address, uint16_t data);

            int write_2ByteTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t data, uint8_t *err = 0);

            int write_2ByteTxOnly(HandlerBase *port, uint8_t id, uint16_t address, uint32_t data);

            int write_2ByteTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint32_t data, uint8_t *err = 0);

            int regWriteTxOnly(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length, uint8_t *data);

            int regWriteTxRx(HandlerBase *port, uint8_t id, uint16_t addr, uint16_t length, uint64_t *data, uint8_t *err = 0);
    };
}

#endif