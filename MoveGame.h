#pragma once

#include <FunBox.h>

class MoveGame : public Game {
public:
  MoveGame();

  void start(FunBox& box) override;
  void update(FunBox& box) override;
  void stop(FunBox& box) override;

private:
  int _x = 3;
  int _y = 3;

  bool _upWasPressed = false;
  bool _downWasPressed = false;
  bool _leftWasPressed = false;
  bool _rightWasPressed = false;

  void handleInput(FunBox& box);
  void draw(FunBox& box);
};