#ifndef INCLUDE_R2D2_UTILS_PKG_TYPES_HPP_
#define INCLUDE_R2D2_UTILS_PKG_TYPES_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace r2d2_commands {
enum class ControlType : uint16_t {
  HOLD = 0x00,
  CONTROL_SPEED = 0x07,
  CONTROL_ANGLE = 0x0A,
  CHECK_ID = 0x800
};
}  // namespace r2d2_commands

namespace r2d2_state {
enum class LockStatus : uint8_t { NONE = 0, LOCKED, UNLOCKED };
enum class WorkMode : uint8_t { NONE = 0, SETUP, AUTO, STOP = 0x80 };
enum class NozzleType : uint8_t { NONE = 0, EMA, BRUSH };

template <typename E>
struct EnumPair {
  E type{};
  std::string key{};

  template <typename T>
  void updateType(const T value) {
    type = static_cast<E>(value);
    updateKey();
  };
  void updateKey();
};

typedef EnumPair<WorkMode> WorkModePair;
typedef EnumPair<LockStatus> LockStatusPair;
typedef EnumPair<NozzleType> NozzleTypePair;

template <>
inline void WorkModePair::updateKey() {
  switch (type) {
    case WorkMode::SETUP:
    case WorkMode::AUTO:
    case WorkMode::STOP:
      key = "none";
      break;
    default:
      key = "";
  }
};
template <>
inline void LockStatusPair::updateKey() {
  switch (type) {
    case LockStatus::LOCKED:
    case LockStatus::UNLOCKED:
      key = "none";
      break;
    default:
      key = "";
  }
};
template <>
inline void NozzleTypePair::updateKey() {
  switch (type) {
    case NozzleType::BRUSH:
      key = "brush";
      break;
    case NozzleType::EMA:
      key = "ema";
      break;
    default:
      key = "";
  }
};
}  // namespace r2d2_state

namespace r2d2_type {
template <typename T, typename T1, typename T2>
struct pipebase_t {
  T1 diameter{};
  T2 thickness{};
  [[nodiscard]] constexpr T radius() const {
    return static_cast<T>(diameter) / T{2} - static_cast<T>(thickness);
  };
};
template <typename T1>
struct payloadbase_t {
  T1 force{};
};
template <typename T, typename T1>
struct nozzlebase_t {
  T1 force_needed{};
  T1 force_tolerance{};
  T r0{};
};
template <typename T, typename T1>
struct jointbase_t {
  T omega{};
  T theta{};
  T1 control_word{};
};
}  // namespace r2d2_type

namespace r2d2_type::callback {
template <typename T>
using pipe_t = pipebase_t<T, uint16_t, uint8_t>;
template <typename T>
using payload_t = payloadbase_t<T>;
template <typename T>
using joint_t = jointbase_t<T, r2d2_commands::ControlType>;

typedef payloadbase_t<int16_t> payload16_t;
typedef jointbase_t<int16_t, uint16_t> joint16_t;
}  // namespace r2d2_type::callback

namespace r2d2_type::config {
template <typename T>
struct payload_t {
  T stiffness{1};
};
template <typename T>
struct joint_t {
  T length{};
  T speed{};
  T angle_offset{};
  T angle_tolerance{};
  std::vector<T> coeffs{};
};
template <typename T>
struct pipe_t {
  T diameter{};
  T thickness{};
  [[nodiscard]] constexpr T radius() const {
    return diameter / T{2} - thickness;
  };
};
template <typename T>
using nozzle_t = nozzlebase_t<T, T>;
}  // namespace r2d2_type::config

#endif  // INCLUDE_R2D2_UTILS_PKG_TYPES_HPP_
