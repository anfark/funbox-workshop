#include <FunBox.h>

#include "Snake.h"
#include "StarFall.h"
#include "Frogger.h"


FunBox box;


// Registriert alle Spiele und initialisiert die FunBox.
void setup() {
  box.addGame<SnakeGame>();
  box.addGame<StarFallGame>();
  box.addGame<FroggerGame>();

  box.setup();
}


// Aktualisiert die FunBox fortlaufend.
void loop() {
  box.update();
}