#include "Frogger.h"


// =====================================================
// KONSTANTEN
// =====================================================

const Color FROG_COLOR = {
  .r = 0,
  .g = 255,
  .b = 0
};


const Color OBSTACLE_COLOR = {
  .r = 255,
  .g = 60,
  .b = 0
};


const Color GOAL_COLOR = {
  .r = 0,
  .g = 80,
  .b = 255
};


const Melody WIN_SOUND = {
  {
    Notes::C5,
    NoteDuration::Eighth
  },
  {
    Notes::E5,
    NoteDuration::Eighth
  },
  {
    Notes::G5,
    NoteDuration::Quarter
  }
};


const Noise HIT_SOUND = {
  NoteDuration::Half
};


const Melody TIMEOUT_SOUND = {
  {
    Notes::E4,
    NoteDuration::Quarter
  },
  {
    Notes::C4,
    NoteDuration::Half
  }
};


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Erzeugt alle Positionen der Zielreihe.
std::vector<Position> goalPositions() {
  std::vector<Position> positions;

  for (
    int x = 0;
    x < BaseGame::bounds.w;
    ++x
  ) {
    positions.push_back({
      .x = x,
      .y =
        BaseGame::bounds.h - 1
    });
  }

  return positions;
}


// Berechnet die Anzahl der verbleibenden Ticks.
uint32_t remainingTicks(
  const FroggerState& state
) {
  if (
    state.ticks >=
    FroggerGame::MAX_TICKS
  ) {
    return 0;
  }

  return
    FroggerGame::MAX_TICKS -
    state.ticks;
}


// Zeichnet einen zentrierten Text auf das Display.
void drawMessage(
  Screen& screen,
  const char* text
) {
  auto& display =
    screen.display();

  display.clearDisplay();

  display.setTextColor(
    SSD1306_WHITE
  );

  display.setTextSize(2);

  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(
    text,
    0,
    0,
    &x1,
    &y1,
    &width,
    &height
  );

  display.setCursor(
    (128 - width) / 2,
    (64 - height) / 2
  );

  display.print(
    text
  );

  display.display();
}


// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Zeichnet den aktuellen Spielzustand und die verbleibende Zeit.
void FroggerRender::state(
  const FroggerState& state
) {
  matrix.clear();

  matrix.drawPixels(
    goalPositions(),
    GOAL_COLOR
  );

  matrix.drawPixels(
    state.obstacles,
    OBSTACLE_COLOR
  );

  matrix.drawPixel(
    state.frog,
    FROG_COLOR
  );

  matrix.show();

  screen.drawNumber(
    remainingTicks(
      state
    )
  );
}


// Spielt den Soundeffekt für eine Kollision mit einem Hindernis.
void FroggerRender::event(
  const DidHitObstacle&
) {
  audio.effect(
    HIT_SOUND
  );
}


// Spielt den Soundeffekt für das Erreichen des Ziels.
void FroggerRender::event(
  const DidReachGoal&
) {
  audio.effect(
    WIN_SOUND
  );
}


// Spielt den Soundeffekt für das Ablaufen der Zeit.
void FroggerRender::event(
  const DidTimeout&
) {
  audio.effect(
    TIMEOUT_SOUND
  );
}


// Zeichnet abhängig vom Endzustand den passenden Abschlussbildschirm.
void FroggerRender::gameOver(
  const FroggerState& state
) {
  if (
    state.frog.y ==
    bounds.h - 1
  ) {
    drawMessage(
      screen,
      "YOU WIN"
    );

    return;
  }

  if (
    state.ticks >=
    FroggerGame::MAX_TICKS
  ) {
    drawMessage(
      screen,
      "TIME UP"
    );

    return;
  }

  screen.drawGameOver(
    0
  );
}