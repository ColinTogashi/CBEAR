//
// Created by msahn on 3/11/21.
//

#include <stdlib.h>
#include <string>
#include <iostream>
#include "packet_manager.h"

#define TX_PKT_MAX_LEN 250
#define RX_PKT_MAX_LEN 250

// Convenient Packet Macros
#define PKT_HEADER0         0
#define PKT_HEADER1         1
#define PKT_ID              2
#define PKT_LENGTH          3
#define PKT_INSTRUCTION     4
#define PKT_ERROR           4
#define PKT_PARAMETER0      5

// Convenient Endian macros
#define DXL_MAKEWORD(a, b)  ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b) ((uint32_t)(((uint16_t)(((uint64_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint64_t)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)       ((uint16_t)(((uint64_t)(l)) & 0xffff))
#define DXL_HIWORD(l)       ((uint16_t)((((uint64_t)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)       ((uint8_t)(((uint64_t)(w)) & 0xff))
#define DXL_HIBYTE(w)       ((uint8_t)((((uint64_t)(w)) >> 8) & 0xff))

using namespace bear;

PacketManager::PacketManager() {}

void PacketManager::BuildPacket(uint8_t *packet) {
  uint8_t checksum = 0;
  uint8_t total_pkt_len = packet[PKT_LENGTH] + 4; // [HEADER0, HEADER1, ID, LENGTH] + ...
  uint8_t write_pkt_len = 0;

  // Make packet header
  packet[PKT_HEADER0] = 0xFF;
  packet[PKT_HEADER1] = 0xFF;

  // Create checksum for the packet
  for (uint16_t idx = 2; idx < total_pkt_len - 1; idx++)
    checksum += packet[idx];
  packet[total_pkt_len - 1] = ~checksum;

//  std::cout << "Packet to be written: " << std::endl;
//  for (int a = 0; a < total_pkt_len; a++)
//    std::cout << int(packet[a]) << " ";
//  std::cout << std::endl;
}

int PacketManager::WritePacket(PortManager *port, uint8_t *packet) {
  uint8_t checksum = 0;
  uint8_t total_pkt_len = packet[PKT_LENGTH] + 4; // [HEADER0, HEADER1, ID, LENGTH] + ...
  uint8_t write_pkt_len = 0;

  // Check to see if the port is in use
  if (port->in_use_)
    return COMM_PORT_BUSY;
  port->in_use_ = true;

  // Check max packet length
  if (total_pkt_len > TX_PKT_MAX_LEN) {
    port->in_use_ = false;
    return COMM_TX_ERROR;
  }

  // Make packet header
  packet[PKT_HEADER0] = 0xFF;
  packet[PKT_HEADER1] = 0xFF;

  // Create checksum for the packet
  for (uint16_t idx = 2; idx < total_pkt_len - 1; idx++)
    checksum += packet[idx];
  packet[total_pkt_len - 1] = ~checksum;

//  std::cout << "Packet to be written: " << std::endl;
//  for (int a = 0; a < total_pkt_len; a++)
//    std::cout << int(packet[a]) << " ";
//  std::cout << std::endl;

//  uint8_t write_pkt_len = 0;
//  uint8_t total_pkt_len = packet[PKT_LENGTH] + 4;
//
//  // Check to see if the port is in use
//  if (port->in_use_)
//    return COMM_PORT_BUSY;
//  port->in_use_ = true;
//
//  // Check max packet length
//  if (total_pkt_len > TX_PKT_MAX_LEN) {
//    port->in_use_ = false;
//    return COMM_TX_ERROR;
//  }

  // Write the actual packet via PortManager
  port->ClearPort();
  write_pkt_len = port->WritePort(packet, total_pkt_len);
  if (total_pkt_len != write_pkt_len) // Write and written should be same
  {
    port->in_use_ = false;
    return COMM_TX_FAIL;
  }

  return COMM_SUCCESS;
}

int PacketManager::ReadPacket(PortManager *port, uint8_t *packet) {
  int result{COMM_TX_FAIL};

  uint8_t checksum = 0;
  uint8_t rx_len = 0;
  uint8_t wait_len = 6; // [HEADER0, HEADER1, ID, LENGTH, ERROR, CHKSUM]

  while (true) {
    rx_len += port->ReadPort(&packet[rx_len], wait_len - rx_len);

    if (rx_len >= wait_len) {
      uint8_t idx = 0;

      // Identify packet header
      for (idx = 0; idx < (rx_len - 1); idx++) {
        if (packet[idx] == 0xFF && packet[idx + 1] == 0xFF)
          break;
      }

      if (idx == 0) // Immediately found (i.e. packet[0], packet[1] are 0xFF, 0xFF
      {
        if (packet[PKT_ID] > 0xFD ||                    // Unavailable ID (max is 0xFC)
            packet[PKT_LENGTH] > RX_PKT_MAX_LEN)        // Unavailable length
        {
          // Remove first byte in packets
          for (uint16_t s = 0; s < rx_len - 1; s++)
            packet[s] = packet[s + 1];
          rx_len -= 1;
          continue;
        }

        // Re-calculate the length of the reading packet if the amount of packets to wait for doesn't match
        if (wait_len != packet[PKT_LENGTH] + PKT_LENGTH + 1) {
          wait_len = packet[PKT_LENGTH] + PKT_LENGTH + 1;
          continue;
        }

        // TODO: Include packet timeout check

        // Calculate checksum
        for (uint16_t jdx = 2; jdx < wait_len - 1; jdx++)
          checksum += packet[jdx];
        checksum = ~checksum;

        // Verify checksum
        if (packet[wait_len - 1] == checksum) {
          result = COMM_SUCCESS;
        } else {
          result = COMM_RX_CORRUPT;
        }
        break;
      } else {
        // Remove unnecessary packets
        for (uint16_t s = 0; s < rx_len - idx; s++)
          packet[s] = packet[idx + s];
        rx_len -= idx;
      }
    } else {
      // TODO: Include timeout check
//      std::cerr << "[ CBEAR ] Couldn't return status packet. Timing out..." << std::endl;
      continue;
    }
  }
  port->in_use_ = false;

//  std::cout << "Packet to be read: " << std::endl;
//  for (int a = 0; a < rx_len; a++)
//    std::cout << int(packet[a]) << " ";
//  std::cout << std::endl;

  return result;
}

int PacketManager::wrPacket(PortManager *port, uint8_t *wpacket, uint8_t *rpacket, uint8_t *error) {
  int result{COMM_TX_FAIL};

  // Write packet
//  BuildPacket(wpacket); // TODO: Make this modular
  result = WritePacket(port, wpacket);
  if (result != COMM_SUCCESS)
    return result;

  // TODO: Include timeout?

  // Read packet
  do {
    result = ReadPacket(port, rpacket);
  } while (result == COMM_SUCCESS && wpacket[PKT_ID] != rpacket[PKT_ID]);

  if (result == COMM_SUCCESS && wpacket[PKT_ID] == rpacket[PKT_ID]) {
    if (error != 0)
      *error = (uint8_t) rpacket[PKT_ERROR];
  }

  return result;
}

int PacketManager::ping(PortManager *port, uint8_t id, uint8_t *error) {
  // Status: Done

  int result{COMM_TX_FAIL};

  // Create packet containers
  uint8_t pkt_tx[6]{};
  uint8_t pkt_rx[RX_PKT_MAX_LEN]{};

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = 2; // No data packets for ping
  pkt_tx[PKT_INSTRUCTION] = INST_PING;

  result = wrPacket(port, pkt_tx, pkt_rx, error);

  return result;
}

int PacketManager::save(bear::PortManager *port, uint8_t id, uint8_t *error) {
  // Status: Done

  int result(COMM_TX_FAIL);

  // Create packet containers
  uint8_t pkt_tx[6]{};
  uint8_t pkt_rx[RX_PKT_MAX_LEN]{};

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = 2; // No data packets for save
  pkt_tx[PKT_INSTRUCTION] = INST_SAVE_CONFIG;

  result = WritePacket(port, pkt_tx);
  port->in_use_ = false;

  return result;
}

/****************************************/
/***** INSTRUCTION PACKET STRUCTURE *****/
/*
 * [0xFF, 0xFF, MOTOR_ID, LENGTH, INSTRUCTION, DATA0, DATA1, ..., DATAN-1, CHKSUM]
 */
/****************************************/
int PacketManager::WriteRegisterTX(bear::PortManager *port, uint8_t id, uint16_t address, uint16_t length,
                                   uint8_t *data, const std::string &sc) {
  // Status: Testing

  int result{COMM_TX_FAIL};

  uint8_t
      *pkt_tx = (uint8_t *) malloc(length + 6); // [0xFF, 0xFF, ID, LENGTH, INST, ..., CHKSUM] is the 6 default bytes

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = length + 3; // [INST, PACKET_LENGTH, CHKSUM], where 3 is [INST, MOTOR_ID, CHKSUM]
  if (sc == "c")
    pkt_tx[PKT_INSTRUCTION] = INST_WRITE_CONFIG;
  else if (sc == "s")
    pkt_tx[PKT_INSTRUCTION] = INST_WRITE_STAT;
  pkt_tx[PKT_PARAMETER0] = (uint8_t) address;

  for (uint16_t s = 0; s < length; s++)
    pkt_tx[PKT_PARAMETER0 + s + 1] = data[s];

  result = WritePacket(port, pkt_tx);
  port->in_use_ = false;

  free(pkt_tx);

  return result;
}

int PacketManager::WriteRegisterTXRX(bear::PortManager *port, uint8_t id, uint16_t address, uint16_t length,
                                     uint8_t *data, uint8_t *error, const std::string &sc) {
  // Status: Testing

  int result{COMM_TX_FAIL};

  uint8_t *pkt_tx = (uint8_t *) malloc(length + 6);
  uint8_t pkt_rx[6]{};

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = length + 3;
  if (sc == "c")
    pkt_tx[PKT_INSTRUCTION] = INST_WRITE_CONFIG;
  else if (sc == "s")
    pkt_tx[PKT_INSTRUCTION] = INST_WRITE_STAT;
  pkt_tx[PKT_PARAMETER0] = (uint8_t) address;

  for (uint16_t s = 0; s < length; s++)
    pkt_tx[PKT_PARAMETER0 + s + 1] = data[s];

  result = wrPacket(port, pkt_tx, pkt_rx, error);

  free(pkt_tx);

  return result;
}

int PacketManager::WriteStatusRegister(bear::PortManager *port, uint8_t id, uint16_t address, uint32_t data,
                                       uint8_t *error) {
  /* Status: WIP
   *
   * - [ ] Create little-endian creating functionality
   */
//    if (address < 3)
  uint8_t data_packed[4] = {DXL_LOBYTE(DXL_LOWORD(data)), DXL_HIBYTE(DXL_LOWORD(data)), DXL_LOBYTE(DXL_HIWORD(data)),
                            DXL_HIBYTE(DXL_HIWORD(data))}; // TODO: Create little-endian creating functionality

//    else
//        uint8_t data_packed[4]
//    uint8_t data_packed[4];
//    data_packed[0] = (data >> 0) & 0xFF;
//    data_packed[1] = (data >> 8) & 0xFF;
//    data_packed[2] = (data >> 16) & 0xFF;
//    data_packed[3] = (data >> 24) & 0xFF;

//  return WriteRegisterTXRX(port, id, address, 4, data_packed, error, "s");
  return WriteRegisterTX(port, id, address, 4, data_packed, "s");
}

int PacketManager::WriteConfigRegister(bear::PortManager *port, uint8_t id, uint16_t address, uint32_t data,
                                       uint8_t *error) {
  /* Status: WIP
   *
   * - [ ] Create little-endian creating functionality
   */
  uint8_t data_packed[4];
  data_packed[0] = (data >> 0) & 0xFF;
  data_packed[1] = (data >> 8) & 0xFF;
  data_packed[2] = (data >> 16) & 0xFF;
  data_packed[3] = (data >> 24) & 0xFF;

//    int result = WriteRegisterTXRX(port, id, address, 4, data_packed, error, "c");
  return WriteRegisterTX(port, id, address, 4, data_packed, "c");
}

int PacketManager::ReadRegisterTX(PortManager *port, uint8_t id, uint16_t address, uint16_t length,
                                  const std::string &sc) {
  // Status: Testing
  int result = COMM_TX_FAIL;

  uint8_t pkt_tx[11]{};

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = 4;
  if (sc == "c")
    pkt_tx[PKT_INSTRUCTION] = INST_READ_CONFIG;
  else if (sc == "s")
    pkt_tx[PKT_INSTRUCTION] = INST_READ_STAT;
  pkt_tx[PKT_PARAMETER0 + 0] = (uint8_t) address;
  pkt_tx[PKT_PARAMETER0 + 1] = (uint8_t) length;

  result = WritePacket(port, pkt_tx);

  // TODO: Set packet timeout

  return result;
}

int PacketManager::ReadRegisterRX(PortManager *port, uint8_t id, uint16_t length, uint8_t *data, uint8_t *error) {
  // Status: Testing

  int result = COMM_RX_FAIL;
  uint8_t *pkt_rx = (uint8_t *) malloc(RX_PKT_MAX_LEN);

  do {
    result = ReadPacket(port, pkt_rx);
  } while (result == COMM_SUCCESS && pkt_rx[PKT_ID] != id);

  if (result == COMM_SUCCESS && pkt_rx[PKT_ID] == id) {
    if (error != 0) {
      *error = (uint8_t) pkt_rx[PKT_ERROR];
    }
    for (uint16_t s = 0; s < length; s++) {
      data[s] = pkt_rx[PKT_PARAMETER0 + s];
    }
  }

  free(pkt_rx);

  return result;
}

int PacketManager::ReadRegisterTXRX(PortManager *port, uint8_t id, uint16_t address, uint16_t length, uint8_t *data,
                                    uint8_t *error, const std::string &sc) {
  // Status: Testing

  int result = COMM_TX_FAIL;

//  uint8_t pkt_tx[7]{};
  uint8_t *pkt_tx = (uint8_t *) malloc(length + 6);
  uint8_t *pkt_rx = (uint8_t *) malloc(RX_PKT_MAX_LEN);

  pkt_tx[PKT_ID] = id;
  pkt_tx[PKT_LENGTH] = (uint8_t) length;
  if (sc == "c")
    pkt_tx[PKT_INSTRUCTION] = INST_READ_CONFIG;
  else if (sc == "s")
    pkt_tx[PKT_INSTRUCTION] = INST_READ_STAT;
  pkt_tx[PKT_PARAMETER0 + 0] = (uint8_t) address;
//  pkt_tx[PKT_PARAMETER0 + 1] = *data;
//    pkt_tx[PKT_PARAMETER0+1] = (uint8_t) length;

  result = wrPacket(port, pkt_tx, pkt_rx, error);
  if (result == COMM_SUCCESS) {
    if (*error != 128) {
      *error = (uint8_t) pkt_rx[PKT_ERROR];
    }
    for (uint16_t s = 0; s < length + 1; s++) {
      data[s] = pkt_rx[PKT_PARAMETER0 + s];
    }
  }

  free(pkt_tx);
  free(pkt_rx);

  return result;
}

int PacketManager::ReadStatusRegister(bear::PortManager *port, uint8_t id, uint16_t address, uint32_t *data,
                                      uint8_t *error) {
  uint8_t data_packed[4]{};

  int result = PacketManager::ReadRegisterTXRX(port, id, address, 3, data_packed, error, "s");

  if (result == COMM_SUCCESS)
    *data = data_packed[0];

  return result;
}

int PacketManager::ReadStatusRegister(bear::PortManager *port, uint8_t id, uint16_t address, float *data,
                                      uint8_t *error) {
  uint8_t data_packed[4]{};

  int result = PacketManager::ReadRegisterTXRX(port, id, address, 3, data_packed, error, "s");

  if (result == COMM_SUCCESS)
    *data = *(float *) &data_packed;

  return result;
}

int PacketManager::ReadConfigRegister(bear::PortManager *port, uint8_t id, uint16_t address, uint32_t *data,
                                      uint8_t *error) {
  uint8_t data_packed[4]{};

  int result = PacketManager::ReadRegisterTXRX(port, id, address, 3, data_packed, error, "c");

  if (result == COMM_SUCCESS)
    *data = data_packed[0];

  return result;
}

int PacketManager::ReadConfigRegister(bear::PortManager *port, uint8_t id, uint16_t address, float *data,
                                      uint8_t *error) {
  uint8_t data_packed[4]{};

  int result = PacketManager::ReadRegisterTXRX(port, id, address, 3, data_packed, error, "c");

  if (result == COMM_SUCCESS)
    *data = *(float *) &data_packed;

  return result;
}