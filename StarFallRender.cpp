#include "StarFall.h"


// =====================================================
// KONSTANTEN
// =====================================================

const Color STAR_COLOR = {
  .r = 255,
  .g = 255,
  .b = 120
};


const Color SHIP_COLOR = {
  .r = 0,
  .g = 120,
  .b = 255
};


const Color SHOT_COLOR = {
  .r = 255,
  .g = 80,
  .b = 0
};


const Melody SHOOT_SOUND = {
  {
    Notes::C5,
    NoteDuration::Eighth
  }
};


const Melody HIT_SOUND = {
  {
    Notes::G5,
    NoteDuration::Eighth
  },
  {
    Notes::C5,
    NoteDuration::Eighth
  }
};


const Noise SHIP_HIT_SOUND = {
  NoteDuration::Half
};


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Zeichnet das Raumschiff an seiner aktuellen Position.
void drawShip(
  Matrix& matrix,
  int shipX
) {
  matrix.drawPixel(
    Position{
      shipX - 1,
      0
    },
    SHIP_COLOR
  );

  matrix.drawPixel(
    Position{
      shipX,
      0
    },
    SHIP_COLOR
  );

  matrix.drawPixel(
    Position{
      shipX + 1,
      0
    },
    SHIP_COLOR
  );

  matrix.drawPixel(
    Position{
      shipX,
      1
    },
    SHIP_COLOR
  );
}


// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Zeichnet den aktuellen Spielzustand auf Matrix und Display.
void StarFallRender::state(
  const StarFallState& state
) {
  matrix.clear();

  matrix.drawPixels(
    state.stars,
    STAR_COLOR
  );

  matrix.drawPixels(
    state.shots,
    SHOT_COLOR
  );

  drawShip(
    matrix,
    state.shipX
  );

  matrix.show();

  screen.drawNumber(
    state.score
  );
}


// Spielt den Soundeffekt für einen abgefeuerten Schuss.
void StarFallRender::event(
  const DidShoot&
) {
  audio.effect(
    SHOOT_SOUND
  );
}


// Spielt den Soundeffekt für einen getroffenen Stern.
void StarFallRender::event(
  const DidHitStar&
) {
  audio.effect(
    HIT_SOUND
  );
}


// Spielt den Soundeffekt für eine Kollision mit dem Schiff.
void StarFallRender::event(
  const DidHitShip&
) {
  audio.effect(
    SHIP_HIT_SOUND
  );
}


// Zeichnet den Game-Over-Bildschirm mit dem finalen Score.
void StarFallRender::gameOver(
  const StarFallState& state
) {
  screen.drawGameOver(
    state.score
  );
}