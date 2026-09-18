#include "StarFall.h"

#include <Arduino.h>
#include <algorithm>


// =====================================================
// KONSTANTEN
// =====================================================

constexpr size_t MIN_STARS = 2;
constexpr size_t MAX_STARS = 4;


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Prüft, ob die angegebene Position Teil des Raumschiffs ist.
bool hitsShip(Position position, int shipX) {
  return
    position == Position{ shipX - 1, 0 } ||
    position == Position{ shipX,     0 } ||
    position == Position{ shipX + 1, 0 } ||
    position == Position{ shipX,     1 };
}


// Prüft, ob ein Stern das Raumschiff berührt.
bool anyStarHitsShip(const std::vector<Position>& stars, int shipX) {
  for (const auto& star : stars) {
    if (hitsShip(star, shipX)) {
      return true;
    }
  }

  return false;
}


// Bewegt alle Positionen um einen Schritt nach unten.
void moveDown(std::vector<Position>& positions) {
  for (auto& position : positions) {
    --position.y;
  }
}


// Bewegt alle Positionen um einen Schritt nach oben.
void moveUp(std::vector<Position>& positions) {
  for (auto& position : positions) {
    ++position.y;
  }
}


// Entfernt alle Positionen unterhalb des Spielfelds.
void removeBelowField(std::vector<Position>& positions) {
  positions.erase(
    std::remove_if(
      positions.begin(),
      positions.end(),
      [](Position position) {
        return position.y < 0;
      }
    ),
    positions.end()
  );
}


// Entfernt alle Positionen oberhalb des Spielfelds.
void removeAboveField(std::vector<Position>& positions, Size bounds) {
  positions.erase(
    std::remove_if(
      positions.begin(),
      positions.end(),
      [bounds](Position position) {
        return position.y >= bounds.h;
      }
    ),
    positions.end()
  );
}


// Erzeugt eine zufällige freie Position in der obersten Reihe.
Position randomStarPosition(const std::vector<Position>& stars, Size bounds) {
  Position position;

  do {
    position = {
      .x = static_cast<int>(random(bounds.w)),
      .y = bounds.h - 1
    };
  } while (contains(stars, position));

  return position;
}


// Prüft, ob abhängig von der aktuellen Anzahl ein neuer Stern entstehen soll.
bool shouldSpawnStar(size_t starCount) {
  if (starCount >= MAX_STARS) {
    return false;
  }

  if (starCount < MIN_STARS) {
    return true;
  }

  return random(4) == 0;
}


// Findet den Index einer Position in einer Liste.
int findPosition(const std::vector<Position>& positions, Position position) {
  const auto it = std::find(
    positions.begin(),
    positions.end(),
    position
  );

  if (it == positions.end()) {
    return -1;
  }

  return static_cast<int>(
    std::distance(positions.begin(), it)
  );
}


// Entfernt das Element am angegebenen Index.
void removeAt(std::vector<Position>& positions, size_t index) {
  positions.erase(
    positions.begin() + index
  );
}


// Entfernt einen getroffenen Schuss und Stern.
bool removeHit(
  std::vector<Position>& shots,
  std::vector<Position>& stars,
  size_t shotIndex,
  Position& hitPosition
) {
  const int starIndex = findPosition(
    stars,
    shots[shotIndex]
  );

  if (starIndex < 0) {
    return false;
  }

  hitPosition = stars[starIndex];

  removeAt(shots, shotIndex);
  removeAt(stars, starIndex);

  return true;
}


// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Erzeugt einen neuen Startzustand für StarFall.
StarFallState StarFallGame::initialState() {
  return {
    .shipX = BaseGame::bounds.w / 2,
    .stars = {},
    .shots = {},
    .ticks = 0,
    .score = 0
  };
}


// Aktualisiert abhängig vom aktuellen Tick entweder Schüsse oder Sterne.
void StarFallUpdate::tick() {
  ++state.ticks;

  if (state.ticks % 2 == 0) {
    moveShots();
  } else {
    moveStars();
    spawnStar();
  }

  detectHits();
}


// Bewegt das Schiff nach links oder rechts innerhalb des Spielfelds.
void StarFallUpdate::move(Direction direction) {
  if (direction == Direction::Left && state.shipX > 0) {
    --state.shipX;
  }

  if (direction == Direction::Right && state.shipX < bounds.w - 1) {
    ++state.shipX;
  }
}


// Erzeugt einen neuen Schuss oberhalb des Raumschiffs.
void StarFallUpdate::click() {
  state.shots.push_back({
    .x = state.shipX,
    .y = 2
  });

  event(DidShoot{});
}


// Prüft, ob ein Stern das Raumschiff getroffen hat.
bool StarFallUpdate::isOver() const {
  return anyStarHitsShip(
    state.stars,
    state.shipX
  );
}


// Bewegt alle Sterne nach unten und entfernt verschwundene Sterne.
void StarFallUpdate::moveStars() {
  moveDown(state.stars);
  removeBelowField(state.stars);
}


// Bewegt alle Schüsse nach oben und entfernt verschwundene Schüsse.
void StarFallUpdate::moveShots() {
  moveUp(state.shots);
  removeAboveField(state.shots, bounds);
}


// Erzeugt bei Bedarf einen neuen Stern an einer freien Position.
void StarFallUpdate::spawnStar() {
  if (!shouldSpawnStar(state.stars.size())) {
    return;
  }

  state.stars.push_back(
    randomStarPosition(
      state.stars,
      bounds
    )
  );
}


// Erkennt Treffer zwischen Schüssen, Sternen und dem Raumschiff.
void StarFallUpdate::detectHits() {
  size_t shotIndex = 0;

  while (shotIndex < state.shots.size()) {
    Position hitPosition;

    if (!removeHit(
      state.shots,
      state.stars,
      shotIndex,
      hitPosition
    )) {
      ++shotIndex;
      continue;
    }

    ++state.score;

    event(DidHitStar{
      .position = hitPosition
    });
  }

  if (anyStarHitsShip(state.stars, state.shipX)) {
    event(DidHitShip{});
  }
}