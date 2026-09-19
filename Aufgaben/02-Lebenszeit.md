# Stufe 2 – Früchte mit Lifetime

## Ziel

Eine Frucht besteht nicht mehr nur aus einer Position.

Sie bekommt zusätzlich eine begrenzte Lebenszeit.

Kurz bevor sie verschwindet, soll sie blinken.

Wenn ihre Lebenszeit abläuft, verschwindet die Frucht und erzeugt:

```cpp id="rot01"
DidRotFruit
```

Neue Früchte werden **nicht direkt als Ersatz erzeugt**. Dafür bleibt weiterhin `spawnFruit()` zuständig.

---

# Snake.h

## Fruit

Neu:

```cpp id="rot02"
struct Fruit {
  Position position;
  uint32_t lifetime;
};
```

## SnakeState

Ändern:

```cpp id="rot03"
struct SnakeState {
  Snake snake;
  Direction direction;
  std::vector<Fruit> fruits;
};
```

## Neues Event

```cpp id="rot04"
struct DidRotFruit {};
```

## SnakeEvent

```cpp id="rot05"
using SnakeEvent =
  std::variant<
    DidEatFruit,
    DidEatSnake,
    DidRotFruit
  >;
```

## SnakeRender

Ergänzen:

```cpp id="rot06"
void event(const DidRotFruit&);
```

---

# SnakeUpdate.cpp

## Konstanten

Ergänzen:

```cpp id="rot07"
constexpr uint32_t FRUIT_LIFETIME = 12;
```

Bestehen bleiben:

```cpp id="rot08"
constexpr size_t MIN_FRUITS = 2;
constexpr size_t MAX_FRUITS = 4;
constexpr long FRUIT_SPAWN_CHANCE = 4;
```

---

## containsFruit()

```cpp id="rot09"
bool containsFruit(
  const std::vector<Fruit>& fruits,
  Position position
) {
  for (const auto& fruit : fruits) {
    if (fruit.position == position) {
      return true;
    }
  }

  return false;
}
```

---

## randomFreePosition()

```cpp id="rot10"
Position randomFreePosition(
  const Snake& snake,
  const std::vector<Fruit>& fruits,
  Size bounds
) {
  Position position;

  do {
    position = randomPosition(bounds);
  } while (
    contains(snake, position) ||
    containsFruit(fruits, position)
  );

  return position;
}
```

---

## createFruit()

```cpp id="rot11"
Fruit createFruit(
  const Snake& snake,
  const std::vector<Fruit>& fruits,
  Size bounds
) {
  return {
    .position = randomFreePosition(
      snake,
      fruits,
      bounds
    ),
    .lifetime = FRUIT_LIFETIME
  };
}
```

---

## spawnFruit()

Die Logik bleibt gleich, erzeugt jetzt aber ein `Fruit`:

```cpp id="rot12"
void spawnFruit(
  const Snake& snake,
  std::vector<Fruit>& fruits,
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
    createFruit(
      snake,
      fruits,
      bounds
    )
  );
}
```

---

## ageFruits()

```cpp id="rot13"
void ageFruits(std::vector<Fruit>& fruits) {
  for (auto& fruit : fruits) {
    if (fruit.lifetime > 0) {
      --fruit.lifetime;
    }
  }
}
```

---

## removeRottenFruits()

Neue Hilfsfunktion:

```cpp id="rot14"
void removeRottenFruits(
  std::vector<Fruit>& fruits,
  const GameUpdate<SnakeState, SnakeEvent>::Emit& event
) {
  for (auto it = fruits.begin(); it != fruits.end();) {
    if (it->lifetime > 0) {
      ++it;
      continue;
    }

    it = fruits.erase(it);

    event(
      DidRotFruit{}
    );
  }
}
```

Wichtig: Hier wird **nur entfernt**. Es wird keine neue Frucht erzeugt.

---

## SnakeGame::initialState()

Snake startet weiterhin mit genau einer Frucht:

```cpp id="rot15"
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

  std::vector<Fruit> fruits;

  fruits.push_back(
    createFruit(
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

Die Reihenfolge ist jetzt wichtig:

```cpp id="rot16"
void SnakeUpdate::tick() {
  auto& snake = state.snake;

  ageFruits(
    state.fruits
  );

  removeRottenFruits(
    state.fruits,
    event
  );

  growSnake(
    snake,
    state.direction
  );

  if (headHitsBody(snake)) {
    event(DidEatSnake{});
    return;
  }

  const auto fruit = std::find_if(
    state.fruits.begin(),
    state.fruits.end(),
    [&snake](const Fruit& fruit) {
      return fruit.position == snake.head;
    }
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

Damit passiert pro Tick:

```text id="rot17"
Früchte altern
↓
verrottete Früchte verschwinden
↓
Schlange bewegt sich
↓
Frucht eventuell essen
↓
spawnFruit() entscheidet über neue Früchte
```

---

# SnakeRender.cpp

## shouldShow()

```cpp id="rot18"
bool shouldShow(const Fruit& fruit) {
  if (fruit.lifetime > 4) {
    return true;
  }

  return fruit.lifetime % 2 == 0;
}
```

## drawGame()

```cpp id="rot19"
for (const auto& fruit : state.fruits) {
  if (!shouldShow(fruit)) {
    continue;
  }

  matrix.drawPixel(
    fruit.position,
    FRUIT_COLOR
  );
}
```

## SnakeRender::event(DidRotFruit)

```cpp id="rot20"
void SnakeRender::event(const DidRotFruit&) {
}
```

Optional kann hier später ein Soundeffekt ergänzt werden.

---

# Ergebnis

Die Verantwortlichkeiten bleiben klar:

```text id="rot21"
ageFruits()
→ reduziert Lifetime

removeRottenFruits()
→ entfernt abgelaufene Früchte
→ erzeugt DidRotFruit

spawnFruit()
→ entscheidet unabhängig,
  ob eine neue Frucht erscheint
```

Das ist sauberer, weil „verrotten“ und „neu spawnen“ zwei getrennte Spielregeln bleiben.
