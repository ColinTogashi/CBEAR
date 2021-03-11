//
// Created by msahn on 3/11/21.
//

#ifndef CBEAR_INCLUDE_PACKET_MANAGER_H_
#define CBEAR_INCLUDE_PACKET_MANAGER_H_

#include <string>
#include "port_manager.h"

// Communication results
#define COMM_SUCCESS 0
#define COMM_PORT_BUSY -1000
#define COMM_TX_FAIL -1001
#define COMM_RX_FAIL -1002
#define COMM_RX_CORRUPT -1003
#define INST_PING 0x01
#define INST_READ_STAT 0x02
#define INST_WRITE_STAT 0x03
#define INST_READ_CONFIG 0x04
#define INST_WRITE_CONFIG 0x05
#define INST_SAVE_CONFIG 0x06
#define COMM_TX_ERROR -2000

namespace bear {
class PacketManager {
 private:
  static PacketManager *unique_instance_;
 public:
  PacketManager();

  /*! \brief Get PacketManager instance.
     *
     * Get PacketManager instance.
     */
  static PacketManager *getPacketManager() { return unique_instance_; }

  /*! \brief Destruct the PacketManager.
   *
   * Destruct the PacketManager.
   */
  virtual ~PacketManager() {}

  /*! \brief Write instruction packet via the PortManager port.
     *
     * After clearing the port via PortManager::clearPort(), the packet is transmitted
     * via PortManager::writePort().
     * @param port PortManager instance.
     * @param packet Packet to be transmitted.
     * @return Result of writing the packet.
     */
  int writePacket(PortManager *port, uint8_t *packet);

  /*! \brief Receive the packet from the PortManager port.
   *
   * Receive the packet from the PortManager port.
   */
  int readPacket(PortManager *port, uint8_t *packet);

  /*! \brief Write and then read the packet from PortManager.
   *
   * Write and then read the packet from PortManager using a combination of
   * PacketManager::writePacket() and PacketManager::readPacket().
   */
  int wrPacket(PortManager *port, uint8_t *wpacket, uint8_t *rpacket, uint8_t *error);

  /*! \brief Ping BEAR.
   *
   * Ping BEAR.
   */
  int ping(PortManager *port, uint8_t id, uint8_t *error);

  /*!
   *
   * @param port PortManager instance
   * @param id Motor ID
   * @param error
   * @return
   */
  int save(PortManager *port, uint8_t id, uint8_t *error);

  /*! \brief Write data to a register.
     *
     * Write data to a register.
     *
     * @param port PortManager instance
     * @param id Motor ID
     * @param address Register address
     * @param length Length of data (not the entire packet)
     * @param data Data to write to register
     * @param sc Type of register "s" for status, "c" for config
     * @return
     */
  int writeRegisterTX(PortManager *port,
                      uint8_t id,
                      uint16_t address,
                      uint16_t length,
                      uint8_t *data,
                      const std::string &sc = "s");

  int writeRegisterTXRX(PortManager *port,
                        uint8_t id,
                        uint16_t address,
                        uint16_t length,
                        uint8_t *data,
                        uint8_t *error = 0,
                        const std::string &sc = "s");

  int writeStatusRegister(PortManager *port, uint8_t id, uint16_t address, uint32_t data, uint8_t *error);

  int writeConfigRegister(PortManager *port, uint8_t id, uint16_t address, uint32_t data, uint8_t *error);

  /*! \brief Initially create the container for status/config packet and transmitted with PacketManager::writePacket().
   *
   * Initially create the container for status/config packet and transmitted with PacketManager::writePacket().
   */
  int readRegisterTX(PortManager *port, uint8_t id, uint16_t address, uint16_t length, const std::string &sc = "s");

  /*! \brief Receive the packet and parse the data in the packet.
   *
   * Receive the packet and parse the data in the packet.
   */
  int readRegisterRX(PortManager *port, uint8_t id, uint16_t length, uint8_t *data, uint8_t *error = 0);

  /*! \brief Transmits the instruction packet and reads the data from the received packet.
   *
   * Transmits the instruction packet and reads the data from the received packet.
   */
  int readRegisterTXRX(PortManager *port, uint8_t id, uint16_t address, uint16_t length, uint8_t *data,
                       uint8_t *error = 0,
                       const std::string &sc = "s");

  int readStatusRegister(PortManager *port, uint8_t id, uint16_t address, uint32_t *data, uint8_t *error = 0);

  int readStatusRegister(PortManager *port, uint8_t id, uint16_t address, float *data, uint8_t *error = 0);

  int readConfigRegister(PortManager *port, uint8_t id, uint16_t address, uint32_t *data, uint8_t *error = 0);

  int readConfigRegister(PortManager *port, uint8_t id, uint16_t address, float *data, uint8_t *error = 0);

  float hexToFloat32(uint8_t *val);
}; // class PacketManager
} // namespace bear

#endif //CBEAR_INCLUDE_PACKET_MANAGER_H_
