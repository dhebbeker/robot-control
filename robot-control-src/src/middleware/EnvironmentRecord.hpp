#ifndef ROBOT_CONTROL_SRC_ENVIRONMENTRECORD_HPP_
#define ROBOT_CONTROL_SRC_ENVIRONMENTRECORD_HPP_

#include <cstddef>
#include <cstdint>

using Distance = std::int16_t; //!< in [mm]
struct Position
{
  Distance x, y;
  bool operator==(const Position& rhs) const
  {
    return x == rhs.x && y == rhs.y;
  }
  bool operator!=(const Position& rhs) const
  {
    return !(*this == rhs);
  }
};

struct EnvironmentRecord {
  static constexpr std::size_t numberOfPositions = 50;
  Position positions[numberOfPositions] = { {0,0} };
  std::size_t positionIndex = 0;
};

#endif /* ROBOT_CONTROL_SRC_ENVIRONMENTRECORD_HPP_ */
