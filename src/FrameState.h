#ifndef FRAMESTATE_H
#define FRAMESTATE_H

#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"

class FrameState {
public:
  FrameState();
  FrameState(FrameState &&) = default;
  FrameState(const FrameState &) = default;
  FrameState &operator=(FrameState &&) = default;
  FrameState &operator=(const FrameState &) = default;
  ~FrameState();

  float getDeltaTime();
  sf::Vector2<int> getInputAxis();
  bool getReset();
  bool getWin();

  void update();
  void endLevel(bool isWin);

  bool checkLevelCollision(int x, int y);
private:
  sf::Clock clock;
  float deltaTime;

  sf::Vector2<int> input;
  sf::Vector2<int> updateInputAxis();

  bool resetLevel;
  bool win;
};
#endif // FRAMESTATE_H
