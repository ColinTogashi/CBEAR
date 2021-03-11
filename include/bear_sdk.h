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
  uint32_t getID(uint8_t mID);
  bool SetID(uint8_t mID, uint32_t val);

  uint32_t getMode(uint8_t mID);
  bool SetMode(uint8_t mID, uint32_t val);

  uint32_t getBaudrate(uint8_t mID);
  bool SetBaudrate(uint8_t mID, uint32_t val);

  float GetHomingOffset(uint8_t mID);
  bool SetHomingOffset(uint8_t mID, float val);

  float GetPGainId(uint8_t mID);
  bool SetPGainId(uint8_t mID, float val);


//    int setDGainId() {};
//    template<typename First, typename Second, typename ... Args>
//    int setDGainId(First mID, Second val, Args &... rest);


  /* ************************************* *
   * Getters/Setters for status registers. *
   * ************************************* */


 private:
  const char *portName_;
  int baudrate_;
  uint8_t bear_error;
  bear::PacketManager packetManager_;
  bear::PortManager portManager_;

  uint32_t floatToUint32(float input);
}; // class BEAR
} // namespace BEAR

#endif //CBEAR_INCLUDE_BEAR_SDK_H_
