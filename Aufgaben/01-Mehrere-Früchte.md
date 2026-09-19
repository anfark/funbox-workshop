# Stufe 1 – Mehrere Früchte

## Ziel

Snake soll nicht mehr nur eine Frucht haben, sondern mehrere.

Die Anzahl der Früchte wird durch zwei Konstanten begrenzt:

```cpp
constexpr size_t MIN_FRUITS = 2;
constexpr size_t MAX_FRUITS = 4;
```

Das Spiel startet trotzdem weiterhin mit genau **einer Frucht**.

Während des Spiels gilt:

```text
weniger als MIN_FRUITS
→ garantiert neue Frucht

zwischen MIN_FRUITS und MAX_FRUITS
→ zufälliger Spawn

MAX_FRUITS erreicht
→ keine neue Frucht
```

---

# Snake.h

## SnakeState

Aus:

```cpp
struct SnakeState {
  Snake snake;
  Direction direction;
  Position fruit;
};
```

wird:

```cpp
struct SnakeState {
  Snake snake;
  Direction direction;
  std::vector<Position> fruits;
};
```

## SnakeUpdate

`nextFruit()` wird nicht mehr benötigt.

Löschen:

```cpp
private:
  Position nextFruit() const;
```

---

# SnakeUpdate.cpp

## Konstanten

Ergänzen:

```cpp
constexpr size_t MIN_FRUITS = 2;
constexpr size_t MAX_FRUITS = 4;

constexpr long FRUIT_SPAWN_CHANCE = 4;
```

`FRUIT_SPAWN_CHANCE = 4` bedeutet eine Chance von `1 zu 4`.

---

## randomFreePosition()

Die Funktion muss jetzt zusätzlich prüfen, ob bereits eine Frucht auf der Position liegt.

```cpp
Position randomFreePosition(
  const Snake& snake,
  const std::vector<Position>& fruits,
  Size bounds
) {
  Position position;

  do {
    position = randomPosition(bounds);
  } while (
    contains(snake, position) ||
    contains(fruits, position)
  );

  return position;
}
```

---

## spawnFruit()

Neue Hilfsfunktion:

```cpp
void spawnFruit(
  const Snake& snake,
  std::vector<Position>& fruits,
  Size bounds
) {
  if (fruits.size() >= MAX_FRUITS) {
    return;
  }

  const bool mustSpawn =
    fruits.size() < MIN_FRUITS;

  if (
    !mustSpawn &&
    random(FRUIT_SPAWN_CHANCE) != 0
  ) {
    return;
  }

  fruits.push_back(
    randomFreePosition(
      snake,
      fruits,
      bounds
    )
  );
}
```

---

## SnakeGame::initialState()

Snake startet weiterhin mit genau einer Frucht.

```cpp
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

  std::vector<Position> fruits;

  fruits.push_back(
    randomFreePosition(
      snake,
      fruits,
      BaseGame::bounds
    )
  );

  return {
    .snake = snake,
    .direction = Direction::Down,
    .fruits = fruits
  };
}
```

---

## SnakeUpdate::tick()

Die bisherige Prüfung:

```cpp
if (snake.head == state.fruit) {
  event(DidEatFruit{});
  state.fruit = nextFruit();
  return;
}
```

wird ersetzt.

```cpp
void SnakeUpdate::tick() {
  auto& snake = state.snake;

  growSnake(snake, state.direction);

  if (headHitsBody(snake)) {
    event(DidEatSnake{});
    return;
  }

  const auto fruit = std::find(
    state.fruits.begin(),
    state.fruits.end(),
    snake.head
  );

  if (fruit != state.fruits.end()) {
    state.fruits.erase(fruit);
    event(DidEatFruit{});
  } else {
    shrinkSnake(snake);
  }

  spawnFruit(
    state.snake,
    state.fruits,
    bounds
  );
}
```

Falls noch nicht vorhanden:

```cpp
#include <algorithm>
```

---

# SnakeRender.cpp

## drawGame()

Aus:

```cpp
matrix.drawPixel(
  state.fruit,
  FRUIT_COLOR
);
```

wird:

```cpp
matrix.drawPixels(
  state.fruits,
  FRUIT_COLOR
);
```

---

# Ergebnis

Der State entwickelt sich von:

```cpp
Position fruit;
```

zu:

```cpp
std::vector<Position> fruits;
```

Damit haben wir das erste Mal sichtbar die Datenstruktur des Spiels erweitert.
