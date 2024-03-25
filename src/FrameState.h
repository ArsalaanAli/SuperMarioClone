#include "SFML/System/Vector2.hpp"

struct FrameState {
  /**
   * @brief The current time since the last frame.
   */
  float deltaTime;

  /**
   * @brief The input axis as a 2D vector.
   */
  sf::Vector2<int> input;
};
