#include <FunBox.h>

FunBox box;

void setup() {
  Serial.begin(115200);

  box.setup();

  Serial.println("FunBox ready!");

  // Matrix testen
  auto& matrix = box.matrix().display();

  matrix.clear();
  matrix.drawPixel(
    3,
    3,
    matrix.Color(0, 255, 0)
  );
  matrix.show();

  // OLED testen
  auto& screen = box.screen().display();

  screen.clearDisplay();
  screen.setCursor(0, 0);
  screen.println("FunBox Workshop");
  screen.println();
  screen.println("Library works!");
  screen.display();

  // Audio testen
  //box.audio().tone(440);
}

void loop() {
  box.update();
}