//
// Created by msahn on 3/11/21.
//

#ifndef CBEAR_INCLUDE_BEAR_MACRO_H_
#define CBEAR_INCLUDE_BEAR_MACRO_H_

#include <stdint.h>

namespace bear_macro{
  const uint8_t ID{0};
  const uint8_t MODE{1};
  const uint8_t BAUDRATE{2};
  const uint8_t HOMING_OFFSET{3};
  const uint8_t P_GAIN_ID{4};
  const uint8_t I_GAIN_ID{5};
  const uint8_t D_GAIN_ID{6};
  const uint8_t P_GAIN_IQ{7};
  const uint8_t I_GAIN_IQ{8};
  const uint8_t D_GAIN_IQ{9};
  const uint8_t P_GAIN_VEL{10};
  const uint8_t I_GAIN_VEL{11};
  const uint8_t D_GAIN_VEL{12};
  const uint8_t P_GAIN_POS{13};
  const uint8_t I_GAIN_POS{14};
  const uint8_t D_GAIN_POS{15};
  const uint8_t P_GAIN_FORCE{16};
  const uint8_t I_GAIN_FORCE{17};
  const uint8_t D_GAIN_FORCE{18};
  const uint8_t LIMIT_ID_MAX{19};
  const uint8_t LIMIT_IQ_MAX{20};
  const uint8_t LIMIT_VEL_MAX{21};
  const uint8_t LIMIT_POS_MIN{22};
  const uint8_t LIMIT_POS_MAX{23};
  const uint8_t MIN_VOLTAGE{24};
  const uint8_t MAX_VOLTAGE{25};
  const uint8_t LOW_VOLTAGE_WARNING{26};
  const uint8_t TEMP_LIMIT_LOW{27};
  const uint8_t TEMP_LIMIT_HIGH{28};

  const uint8_t TORQUE_ENABLE{0};
  const uint8_t HOMING_COMPLETE{1};
  const uint8_t GOAL_ID{2};
  const uint8_t GOAL_IQ{3};
  const uint8_t GOAL_VELOCITY{4};
  const uint8_t GOAL_POSITION{5};
  const uint8_t PRESENT_ID{6};
  const uint8_t PRESENT_IQ{7};
  const uint8_t PRESENT_VELOCITY{8};
  const uint8_t PRESENT_POSITION{9};
  const uint8_t INPUT_VOLTAGE{10};
  const uint8_t WINDING_TEMPERATURE{11};
  const uint8_t POWERSTAGE_TEMPERATURE{12};
  const uint8_t IC_TEMPERATURE{13};
  const uint8_t ERROR_STATUS{14};
  const uint8_t PACKED_IQ_TEMP{15};
}

namespace bear {
namespace registers {
  // Config registers
  const uint8_t ID{0};
  const uint8_t MODE{1};
  const uint8_t BAUDRATE{2};
  const uint8_t HOMING_OFFSET{3};
  const uint8_t P_GAIN_ID{4};
  const uint8_t I_GAIN_ID{5};
  const uint8_t D_GAIN_ID{6};
  const uint8_t P_GAIN_IQ{7};
  const uint8_t I_GAIN_IQ{8};
  const uint8_t D_GAIN_IQ{9};
  const uint8_t P_GAIN_VEL{10};
  const uint8_t I_GAIN_VEL{11};
  const uint8_t D_GAIN_VEL{12};
  const uint8_t P_GAIN_POS{13};
  const uint8_t I_GAIN_POS{14};
  const uint8_t D_GAIN_POS{15};
  const uint8_t P_GAIN_FORCE{16};
  const uint8_t I_GAIN_FORCE{17};
  const uint8_t D_GAIN_FORCE{18};
  const uint8_t LIMIT_ID_MAX{19};
  const uint8_t LIMIT_IQ_MAX{20};
  const uint8_t LIMIT_VEL_MAX{21};
  const uint8_t LIMIT_POS_MIN{22};
  const uint8_t LIMIT_POS_MAX{23};
  const uint8_t MIN_VOLTAGE{24};
  const uint8_t MAX_VOLTAGE{25};
  const uint8_t LOW_VOLTAGE_WARNING{26};
  const uint8_t TEMP_LIMIT_LOW{27};
  const uint8_t TEMP_LIMIT_HIGH{28};
  // Status registers
  const uint8_t TORQUE_ENABLE{0};
  const uint8_t HOMING_COMPLETE{1};
  const uint8_t GOAL_ID{2};
  const uint8_t GOAL_IQ{3};
  const uint8_t GOAL_VELOCITY{4};
  const uint8_t GOAL_POSITION{5};
  const uint8_t PRESENT_ID{6};
  const uint8_t PRESENT_IQ{7};
  const uint8_t PRESENT_VELOCITY{8};
  const uint8_t PRESENT_POSITION{9};
  const uint8_t INPUT_VOLTAGE{10};
  const uint8_t WINDING_TEMPERATURE{11};
  const uint8_t POWERSTAGE_TEMPERATURE{12};
  const uint8_t IC_TEMPERATURE{13};
  const uint8_t ERROR_STATUS{14};
  const uint8_t PACKED_IQ_TEMP{15};
} // namespace registers
namespace error {
    constexpr static uint8_t COMMUNICATION = 0x01;
    constexpr static uint8_t OVER_TEMPERATURE = 0x02;
    constexpr static uint8_t ABSOLUTE_POSITION = 0x04;
    constexpr static uint8_t WATCHDOG = 0x08;
    constexpr static uint8_t JOINT_LIMIT = 0x10;
    constexpr static uint8_t HARDWARE_FAULT = 0x20;
    constexpr static uint8_t INITIALIZATION = 0x40;
    constexpr static uint8_t NORMAL = 0x80;
}
namespace mode {
    constexpr static uint8_t FORCE = 0;
    constexpr static uint8_t VELOCITY = 1;
    constexpr static uint8_t POSITION = 2;
    constexpr static uint8_t DIRECT_FORCE = 3;
} // namespace mode
namespace enable {
    constexpr static uint8_t OFF = 0;
    constexpr static uint8_t ON = 1;
    constexpr static uint8_t ERROR = 2;
    constexpr static uint8_t DAMPING = 3;
} // namespace enable

} // namespace bear

#endif //CBEAR_INCLUDE_BEAR_MACRO_H_
