//
// Created by msahn on 3/11/21.
//

#ifndef CBEAR_INCLUDE_BEAR_SDK_H_
#define CBEAR_INCLUDE_BEAR_SDK_H_

#include <string>
#include "packet_manager.h"
#include "port_manager.h"

namespace bear {
class BEAR {
 public:
  BEAR(const char *portName, int baudrate);

  void connect();

  /*! Observe the error code present in the chain.
   *
   * @return
   */
  uint8_t GetErrorCode();

  /*! Attempt to ping a motor.
   *
   * @param mID Motor ID
   * @return bool
   */
  int ping(uint8_t mID);

  /*! Save the configuration registers.
   *
   * @param mID Motor ID
   * @return bool
   */
  int save(uint8_t mID);

  /* ******************************************** *
   * Getters/Setters for configuration registers. *
   * ******************************************** */
  uint32_t GetID(uint8_t mID);
  bool SetID(uint8_t mID, uint32_t val);

  uint32_t GetMode(uint8_t mID);
  bool SetMode(uint8_t mID, uint32_t val);

  uint32_t GetBaudrate(uint8_t mID);
  bool SetBaudrate(uint8_t mID, uint32_t val);

  float GetHomingOffset(uint8_t mID);
  bool SetHomingOffset(uint8_t mID, float val);

  float GetPGainId(uint8_t mID);
  bool SetPGainId(uint8_t mID, float val);

  float GetIGainId(uint8_t mID);
  bool SetIGainId(uint8_t mID, float val);

  float GetDGainId(uint8_t mID);
  bool SetDGainId(uint8_t mID, float val);

  float GetPGainIq(uint8_t mID);
  bool SetPGainIq(uint8_t mID, float val);

  float GetIGainIq(uint8_t mID);
  bool SetIGainIq(uint8_t mID, float val);

  float GetDGainIq(uint8_t mID);
  bool SetDGainIq(uint8_t mID, float val);

  float GetPGainVelocity(uint8_t mID);
  bool SetPGainVelocity(uint8_t mID, float val);

  float GetIGainVelocity(uint8_t mID);
  bool SetIGainVelocity(uint8_t mID, float val);

  float GetDGainVelocity(uint8_t mID);
  bool SetDGainVelocity(uint8_t mID, float val);

  float GetPGainPosition(uint8_t mID);
  bool SetPGainPosition(uint8_t mID, float val);

  float GetIGainPosition(uint8_t mID);
  bool SetIGainPosition(uint8_t mID, float val);

  float GetDGainPosition(uint8_t mID);
  bool SetDGainPosition(uint8_t mID, float val);

  float GetPGainDirectForce(uint8_t mID);
  bool SetPGainDirectForce(uint8_t mID, float val);

  float GetIGainDirectForce(uint8_t mID);
  bool SetIGainDirectForce(uint8_t mID, float val);

  float GetDGainDirectForce(uint8_t mID);
  bool SetDGainDirectForce(uint8_t mID, float val);

  float GetLimitAccMax(uint8_t mID);
  bool SetLimitAccMax(uint8_t mID, float val);

//    int setDGainId() {};
//    template<typename First, typename Second, typename ... Args>
//    int setDGainId(First mID, Second val, Args &... rest);


  /* ************************************* *
   * Getters/Setters for status registers. *
   * ************************************* */
  uint32_t GetTorqueEnable(uint8_t mID);
  bool SetTorqueEnable(uint8_t mID, uint32_t val);

  float GetGoalId(uint8_t mID);
  bool SetGoalId(uint8_t mID, float val);

  float GetGoalIq(uint8_t mID);
  bool SetGoalIq(uint8_t mID, float val);

  float GetGoalVelocity(uint8_t mID);
  bool SetGoalVelocity(uint8_t mID, float val);

  float GetGoalPosition(uint8_t mID);
  bool SetGoalPosition(uint8_t mID, float val);

  float GetPresentId(uint8_t mID);

  float GetPresentIq(uint8_t mID);

  float GetPresentVelocity(uint8_t mID);

  float GetPresentPosition(uint8_t mID);

  float GetInputVoltage(uint8_t mID);

  float GetWindingTemperature(uint8_t mID);

  float GetPowerstageTemperature(uint8_t mID);

  float GetICTemperature(uint8_t mID);

  uint32_t GetErrorStatus(uint8_t mID);

  uint32_t GetPackedIqAndTemps(uint8_t mID);

  /* ****************** *
   * Utility functions. *
   * ****************** */
  static uint32_t floatToUint32(float input);


 private:
  const char *portName_;
  int baudrate_;
  uint8_t bear_error;
  bear::PacketManager packetManager_;
  bear::PortManager portManager_;
}; // class BEAR
} // namespace BEAR

#endif //CBEAR_INCLUDE_BEAR_SDK_H_
