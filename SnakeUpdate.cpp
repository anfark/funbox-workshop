#include "Snake.h"

#include <Arduino.h>


// =====================================================
// KONSTANTEN
// =====================================================


// =====================================================
// HILFSFUNKTIONEN
// =====================================================

// Prüft, ob die angegebene Position Teil der Schlange ist.
bool contains(const Snake& snake, Position position) {
  return snake.head == position || contains(snake.body, position);
}


// Prüft, ob der Kopf ein Segment des Körpers berührt.
bool headHitsBody(const Snake& snake) {
  return contains(snake.body, snake.head);
}

Position randomPosition(Size bounds) {
  return {
      .x = static_cast<int>(random(bounds.w)),
      .y = static_cast<int>(random(bounds.h))
    };
}

// Erzeugt eine zufällige freie Position innerhalb des Spielfelds.
Position randomFreePosition(const Snake& snake, Size bounds) {
  Position position;

  do {
    position = randomPosition(bounds);
  } while (contains(snake, position));

  return position;
}

// Vergrößert die Schlange um ein Segment in die angegebene Richtung.
void growSnake(Snake& snake, Direction direction) {
  snake.body.insert(snake.body.begin(), snake.head);
  snake.head = (snake.head + direction) % BaseGame::bounds;
}

// Entfernt das letzte Segment der Schlange.
void shrinkSnake(Snake& snake) {
  snake.body.pop_back();
}

// =====================================================
// IMPLEMENTIERUNG
// =====================================================

// Erzeugt einen neuen Startzustand für Snake.
SnakeState SnakeGame::initialState() {
  const Position head = {
    .x = BaseGame::bounds.w / 2,
    .y = BaseGame::bounds.h / 2
  };

  const auto seg0 = head + Direction::Up;
  const auto seg1 = seg0 + Direction::Left;
  const auto seg2 = seg1 + Direction::Down;

  const auto snake = Snake {
    .head = head,
    .body = { seg0, seg1, seg2 }
  };

  return {
    .snake = snake,
    .direction = Direction::Down,
    .fruit = randomFreePosition(snake, BaseGame::bounds)
  };
}


// Bewegt die Schlange um einen Schritt und verarbeitet Kollisionen und Früchte.
void SnakeUpdate::tick() {
  auto& snake = state.snake;

  growSnake(snake, state.direction);

  if (headHitsBody(snake)) {
    event(DidEatSnake{});
    return;
  }

  if (snake.head == state.fruit) {
    event(DidEatFruit{});
    state.fruit = nextFruit();
    return;
  }

  shrinkSnake(snake);
}


// Ändert die Bewegungsrichtung, solange keine direkte Umkehr entsteht.
void SnakeUpdate::move(Direction direction) {
  if (state.direction == inverseDirection(direction)) {
    return;
  }

  state.direction = direction;
}


// Prüft, ob sich der Kopf mit dem eigenen Körper überschneidet.
bool SnakeUpdate::isOver() const {
  return headHitsBody(state.snake);
}


// Erzeugt eine neue freie Position für die Frucht.
Position SnakeUpdate::nextFruit() const {
  return randomFreePosition(state.snake, bounds);
}