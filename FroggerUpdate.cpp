#include "Frogger.h"


// =====================================================
// KONSTANTEN
// =====================================================


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Prüft, ob die angegebene Reihe eine schnelle Hindernisreihe ist.
bool isFastLane(
  int y
) {
  return
    y == 6;
}


// Liefert die Bewegungsrichtung der angegebenen Hindernisreihe.
int laneDirection(
  int y
) {
  switch (y) {
    case 1:
      return 1;

    case 3:
      return -1;

    case 4:
      return 1;

    case 6:
      return -1;

    default:
      return 0;
  }
}


// Bewegt eine Position horizontal und lässt sie am Spielfeldrand umbrechen.
Position moveHorizontal(
  Position position,
  int direction,
  Size bounds
) {
  position.x +=
    direction;

  if (
    position.x < 0
  ) {
    position.x =
      bounds.w - 1;
  }

  if (
    position.x >=
    bounds.w
  ) {
    position.x = 0;
  }

  return position;
}


// Prüft, ob der Frosch ein Hindernis berührt.
bool hitsObstacle(
  const FroggerState& state
) {
  return contains(
    state.obstacles,
    state.frog
  );
}


// Prüft, ob der Frosch die Zielreihe erreicht hat.
bool reachedGoal(
  const FroggerState& state
) {
  return
    state.frog.y ==
    BaseGame::bounds.h - 1;
}


// Prüft, ob die verfügbare Zeit abgelaufen ist.
bool timedOut(
  const FroggerState& state
) {
  return
    state.ticks >=
    FroggerGame::MAX_TICKS;
}


// Erzeugt die Hindernisse für den initialen Spielzustand.
std::vector<Position> initialObstacles() {
  return {
    {1, 1},
    {5, 1},

    {1, 3},
    {2, 3},

    {5, 3},
    {6, 3},

    {0, 4},
    {4, 4},

    {1, 6},
    {2, 6},

    {5, 6},
    {6, 6}
  };
}


// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Erzeugt einen neuen Startzustand für Frogger.
FroggerState FroggerGame::initialState() {
  return {
    .frog = {
      .x =
        BaseGame::bounds.w / 2,
      .y = 0
    },

    .obstacles =
      initialObstacles(),

    .ticks = 0
  };
}


// Aktualisiert die Spielzeit und bewegt die Hindernisse.
void FroggerUpdate::tick() {
  ++state.ticks;

  moveObstacles();
  detectEnd();
}


// Bewegt den Frosch um ein Feld innerhalb des Spielfelds.
void FroggerUpdate::move(
  Direction direction
) {
  const Position next =
    state.frog +
    direction;

  if (
    next.isOutside(
      bounds
    )
  ) {
    return;
  }

  state.frog =
    next;

  detectEnd();
}


// Prüft, ob das Spiel durch Treffer, Ziel oder Zeitablauf beendet ist.
bool FroggerUpdate::isOver() const {
  return
    hitsObstacle(state) ||
    reachedGoal(state) ||
    timedOut(state);
}


// Bewegt normale und schnelle Hindernisse abhängig vom aktuellen Tick.
void FroggerUpdate::moveObstacles() {
  for (
    auto& obstacle :
    state.obstacles
  ) {
    const int direction =
      laneDirection(
        obstacle.y
      );

    if (
      direction == 0
    ) {
      continue;
    }

    const bool shouldMove =
      isFastLane(
        obstacle.y
      )
      ||
      state.ticks % 2 == 0;

    if (
      !shouldMove
    ) {
      continue;
    }

    obstacle =
      moveHorizontal(
        obstacle,
        direction,
        bounds
      );
  }
}


// Erzeugt das passende Event, sobald eine Endbedingung erreicht wurde.
void FroggerUpdate::detectEnd() {
  if (
    hitsObstacle(state)
  ) {
    event(
      DidHitObstacle{}
    );

    return;
  }

  if (
    reachedGoal(state)
  ) {
    event(
      DidReachGoal{}
    );

    return;
  }

  if (
    timedOut(state)
  ) {
    event(
      DidTimeout{}
    );
  }
}