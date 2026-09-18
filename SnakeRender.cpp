#include "Snake.h"


// =====================================================
// KONSTANTEN
// =====================================================

const Color FRUIT_COLOR = {
  .r = 255,
  .g = 0,
  .b = 0
};


const Color HEAD_COLOR = {
  .r = 0,
  .g = 255,
  .b = 0
};


const Color BODY_COLOR = {
  .r = 0,
  .g = 80,
  .b = 0
};


const Melody EAT_SOUND = {
  {
    Notes::E5,
    NoteDuration::Eighth
  },
  {
    Notes::A5,
    NoteDuration::Eighth
  }
};


const Noise COLLISION_SOUND = {
  NoteDuration::Half
};


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Berechnet den aktuellen Score aus der zusätzlichen Körperlänge.
uint32_t score(const SnakeState& state) {
  return state.snake.body.size() - 3;
}


// Zeichnet Schlange und Frucht auf die Matrix.
void drawGame(Matrix& matrix, const SnakeState& state) {
  matrix.clear();

  matrix.drawPixel(
    state.fruit,
    FRUIT_COLOR
  );

  matrix.drawPixels(
    state.snake.body,
    BODY_COLOR
  );

  matrix.drawPixel(
    state.snake.head,
    HEAD_COLOR
  );

  matrix.show();
}


// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Zeichnet den aktuellen Spielzustand und den Score.
void SnakeRender::state(const SnakeState& state) {
  drawGame(matrix, state);

  screen.drawNumber(
    score(state)
  );
}


// Spielt den Soundeffekt für eine gefressene Frucht.
void SnakeRender::event(const DidEatFruit&) {
  audio.effect(EAT_SOUND);
}


// Spielt den Soundeffekt für eine Kollision mit dem eigenen Körper.
void SnakeRender::event(const DidEatSnake&) {
  audio.effect(COLLISION_SOUND);
}


// Zeichnet den Game-Over-Bildschirm mit dem finalen Score.
void SnakeRender::gameOver(const SnakeState& state) {
  screen.drawGameOver(score(state));
}