#include <FunBox.h>

#include "MoveGame.h"

FunBox box;
MoveGame moveGame;

void setup() {
  Serial.begin(115200);

  box.addGame(moveGame);
  box.setup();
}

void loop() {
  box.update();
}