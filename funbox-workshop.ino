#include <FunBox.h>

#include "Snake.h"


FunBox box;


SnakeConfig snakeConfig = {
  .bounds = {
    .w = 8,
    .h = 8
  }
};


SnakeGame snake(
  snakeConfig,
  box.matrix(),
  box.screen(),
  box.audio()
);


GameDescription snakeDescription(
  "Snake",
  snake
);


void setup() {
  box.addGame(snakeDescription);

  box.setup();
}


void loop() {
  box.update();
}