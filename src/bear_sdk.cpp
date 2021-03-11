//
// Created by msahn on 3/11/21.
//

#include <string>
#include "bear_sdk.h"

// Configuration Registers
#define CR_ID                       0
#define CR_MODE                     1
#define CR_BAUDRATE                 2
#define CR_HOMING_OFFSET            3
#define CR_P_GAIN_ID                4
#define CR_I_GAIN_ID                5
#define CR_D_GAIN_ID                6
#define CR_P_GAIN_IQ                7
#define CR_I_GAIN_IQ                8
#define CR_D_GAIN_IQ                9
#define CR_P_GAIN_VEL               10
#define CR_I_GAIN_VEL               11
#define CR_D_GAIN_VEL               12
#define CR_P_GAIN_POS               13
#define CR_I_GAIN_POS               14
#define CR_D_GAIN_POS               15
#define CR_P_GAIN_FORCE             16
#define CR_I_GAIN_FORCE             17
#define CR_D_GAIN_FORCE             18
#define CR_LIMIT_ID_MAX             19
#define CR_LIMIT_IQ_MAX             20
#define CR_LIMIT_VEL_MAX            21
#define CR_LIMIT_POS_MIN            22
#define CR_LIMIT_POS_MAX            23
#define CR_MIN_VOLTAGE              24
#define CR_MAX_VOLTAGE              25
#define CR_LOW_VOLTAGE_WARNING      26
#define CR_TEMP_LIMIT_LOW           27
#define CR_TEMP_LIMIT_HIGH          28

// Status Registers
#define SR_TORQUE_ENABLE            0
#define SR_HOMING_COMPLETE          1
#define SR_GOAL_ID                  2
#define SR_GOAL_IQ                  3
#define SR_GOAL_VELOCITY            4
#define SR_GOAL_POSITION            5
#define SR_PRESENT_ID               6
#define SR_PRESENT_IQ               7
#define SR_PRESENT_VELOCITY         8
#define SR_PRESENT_POSITION         9
#define SR_INPUT_VOLTAGE            10
#define SR_WINDING_TEMPERATURE      11
#define SR_POWERSTAGE_TEMPERATURE   12
#define SR_IC_TEMPERATURE           13
#define SR_ERROR_STATUS             14
#define SR_WARNING_STATUS           15

// Return Error Codes
#define RET_ERR                     999

using namespace bear;

BEAR::BEAR(const char *portName, int baudrate)
    : portName_{portName},
      baudrate_{baudrate},
      packetManager_{bear::PacketManager()},
      portManager_{bear::PortManager(portName_, baudrate)} {}

uint8_t BEAR::GetErrorCode() {
  return bear_error;
}

int BEAR::ping(uint8_t mID) {
  return (packetManager_.ping(&portManager_, mID, &bear_error) == COMM_SUCCESS);
}

int BEAR::save(uint8_t mID) {
  return (packetManager_.save(&portManager_, mID, &bear_error) == COMM_SUCCESS);
}

/* ******************************************** *
 * Getters/Setters for configuration registers. *
 * ******************************************** */
uint32_t BEAR::getID(uint8_t mID) {
  uint32_t retVal;
  if (packetManager_.ReadConfigRegister(&portManager_, mID, CR_ID, &retVal, &bear_error) != COMM_SUCCESS)
    retVal = RET_ERR;
  return retVal;
}

bool BEAR::SetID(uint8_t mID, uint32_t val) {
  return (packetManager_.WriteConfigRegister(&portManager_, mID, CR_ID, val, &bear_error) == COMM_SUCCESS);
}

uint32_t BEAR::getMode(uint8_t mID) {
  uint32_t retVal;
  if (packetManager_.ReadConfigRegister(&portManager_, mID, CR_MODE, &retVal, &bear_error) != COMM_SUCCESS)
    retVal = RET_ERR;
  return retVal;
}

bool BEAR::SetMode(uint8_t mID, uint32_t val) {
  return (packetManager_.WriteConfigRegister(&portManager_, mID, CR_MODE, val, &bear_error) == COMM_SUCCESS);
}

uint32_t BEAR::getBaudrate(uint8_t mID) {
  uint32_t retVal;
  if (packetManager_.ReadConfigRegister(&portManager_, mID, CR_BAUDRATE, &retVal, &bear_error) != COMM_SUCCESS)
    retVal = RET_ERR;
  return retVal;
}

bool BEAR::SetBaudrate(uint8_t mID, uint32_t val) {
  return (packetManager_.WriteConfigRegister(&portManager_, mID, CR_BAUDRATE, val, &bear_error) == COMM_SUCCESS);
}

float BEAR::GetHomingOffset(uint8_t mID) {
  float retVal;
  if (packetManager_.ReadConfigRegister(&portManager_, mID, CR_HOMING_OFFSET, &retVal, &bear_error) != COMM_SUCCESS)
    retVal = RET_ERR;
  return retVal;
}

bool BEAR::SetHomingOffset(uint8_t mID, float val) {
  return (packetManager_.WriteConfigRegister(&portManager_, mID, CR_HOMING_OFFSET, floatToUint32(val), &bear_error)
      == COMM_SUCCESS);
}

float BEAR::GetPGainId(uint8_t mID) {
  float retVal;
  if (packetManager_.ReadConfigRegister(&portManager_, mID, CR_P_GAIN_ID, &retVal, &bear_error) != COMM_SUCCESS)
    retVal = RET_ERR;
  return retVal;
}

bool BEAR::SetPGainId(uint8_t mID, float val) {
  return (packetManager_.WriteConfigRegister(&portManager_, mID, CR_P_GAIN_ID, floatToUint32(val), &bear_error)
      == COMM_SUCCESS);
}

uint32_t BEAR::floatToUint32(float input) {
  static_assert(sizeof(float) == sizeof(uint32_t), "Float and uint32 are not the same size in your setup!");
  auto *pInt = reinterpret_cast<uint32_t *>(&input);
  return *pInt;
}