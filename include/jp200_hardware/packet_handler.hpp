#ifndef PACKET_HANDLER_HPP_
#define PACKET_HANDLER_HPP_

#include "jp200_hardware/handler_base.hpp"
#include <stdio.h>
#include <string>
#include <vector>

#define COMM_SUCCESS 0
#define COMM_PORT_BUSY -1000
#define COMM_TX_FAIL        -1001   
#define COMM_RX_FAIL        -1002   
#define COMM_TX_ERROR       -2000   
#define COMM_RX_WAITING     -3000   
#define COMM_RX_TIMEOUT     -3001   
#define COMM_RX_CORRUPT     -3002   
#define COMM_NOT_AVAILABLE  -9000   

#define PKT_HEADER0             0
#define PKT_HEADER1             1
#define PKT_ID                  2
#define PKT_LENGTH              3
#define PKT_INSTRUCTION         4
#define PKT_ERROR               4
#define PKT_PARAMETER0          5

#define TXPACKET_MAX_LEN    (250)
#define RXPACKET_MAX_LEN    (250)

#define BROADCAST_ID 0xFE
#define MAX_ID 0xFC

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

            int RxPacket(HandlerBase *port, uint8_t *rx_packet, bool skip_stuffing);

            int TxRxPacket(HandlerBase *port, uint8_t *tx_packet, uint8_t *rx_packet, uint8_t *err);

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