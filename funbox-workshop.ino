#include <FunBox.h>

#include "Snake.h"


FunBox box;


SnakeConfig snakeConfig = {
  .bounds = Matrix::SIZE
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