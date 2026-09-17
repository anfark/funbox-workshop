#include "MoveGame.h"

MoveGame::MoveGame()
  : Game("Move") {
}

void MoveGame::start(FunBox& box) {
  _x = 3;
  _y = 3;

  _upWasPressed = false;
  _downWasPressed = false;
  _leftWasPressed = false;
  _rightWasPressed = false;

  draw(box);
}

void MoveGame::update(FunBox& box) {
  handleInput(box);
}

void MoveGame::stop(FunBox& box) {
  auto& matrix = box.matrix().display();

  matrix.clear();
  matrix.show();
}

void MoveGame::handleInput(FunBox& box) {
  auto& stick = box.stick();

  const bool upPressed =
    stick.isPressed(StickKey::UP);

  const bool downPressed =
    stick.isPressed(StickKey::DOWN);

  const bool leftPressed =
    stick.isPressed(StickKey::LEFT);

  const bool rightPressed =
    stick.isPressed(StickKey::RIGHT);


  bool moved = false;


  if (upPressed && !_upWasPressed) {
    if (_y > 0) {
      _y--;
      moved = true;
    }
  }

  if (downPressed && !_downWasPressed) {
    if (_y < 7) {
      _y++;
      moved = true;
    }
  }

  if (leftPressed && !_leftWasPressed) {
    if (_x > 0) {
      _x--;
      moved = true;
    }
  }

  if (rightPressed && !_rightWasPressed) {
    if (_x < 7) {
      _x++;
      moved = true;
    }
  }


  _upWasPressed = upPressed;
  _downWasPressed = downPressed;
  _leftWasPressed = leftPressed;
  _rightWasPressed = rightPressed;


  if (moved) {
    draw(box);
  }
}

void MoveGame::draw(FunBox& box) {
  auto& matrix = box.matrix().display();

  matrix.clear();

  matrix.drawPixel(
    _x,
    _y,
    matrix.Color(255, 255, 255)
  );

  matrix.show();
}