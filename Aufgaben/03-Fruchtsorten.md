# Stufe 3 – Verschiedene Fruchtarten

## Ziel

Unsere Früchte bekommen unterschiedliche Typen:

* 🍎 **Apple** – rot
* 🫐 **Berry** – blau
* 🍌 **Banana** – gelb

Die Fruchtarten unterscheiden sich außerdem in:

* Farbe
* Score
* Lifetime

---

# Snake.h

## FruitType

Neu:

```cpp id="fruit31"
enum class FruitType {
  Apple,
  Berry,
  Banana
};
```

---

## Fruit

Den Typ ergänzen:

```cpp id="fruit32"
struct Fruit {
  Position position;
  FruitType type;
  uint32_t lifetime;
};
```

---

## SnakeState

Da Früchte unterschiedlich viele Punkte geben, bekommt der State einen eigenen Score:

```cpp id="fruit33"
struct SnakeState {
  Snake snake;
  Direction direction;
  std::vector<Fruit> fruits;
  uint32_t score = 0;
};
```

---

# SnakeUpdate.cpp

## randomFruitType()

Erzeugt einen zufälligen Fruchttyp:

```cpp id="fruit34"
FruitType randomFruitType() {
  switch (random(3)) {
    case 0:
      return FruitType::Apple;

    case 1:
      return FruitType::Berry;

    default:
      return FruitType::Banana;
  }
}
```

---

## lifetime()

Legt die Lebenszeit abhängig vom Fruchttyp fest:

```cpp id="fruit35"
uint32_t lifetime(FruitType type) {
  switch (type) {
    case FruitType::Apple:
      return 12;

    case FruitType::Berry:
      return 8;

    case FruitType::Banana:
      return 4;
  }

  return 12;
}
```

---

## score()

Legt die Punkte abhängig vom Fruchttyp fest:

```cpp id="fruit36"
uint32_t score(FruitType type) {
  switch (type) {
    case FruitType::Apple:
      return 1;

    case FruitType::Berry:
      return 2;

    case FruitType::Banana:
      return 5;
  }

  return 0;
}
```

Damit gilt:

| Frucht | Farbe | Lifetime | Score |
| ------ | ----- | -------: | ----: |
| Apple  | Rot   |       12 |     1 |
| Berry  | Blau  |        8 |     2 |
| Banana | Gelb  |        4 |     5 |

Je kürzer eine Frucht verfügbar ist, desto mehr Punkte bringt sie.

---

## createFruit()

Zuerst wird ein zufälliger Typ bestimmt. Aus diesem ergibt sich anschließend die Lifetime:

```cpp id="fruit37"
Fruit createFruit(
  const Snake& snake,
  const std::vector<Fruit>& fruits,
  Size bounds
) {
  const auto type =
    randomFruitType();

  return {
    .position = randomFreePosition(
      snake,
      fruits,
      bounds
    ),
    .type = type,
    .lifetime = lifetime(type)
  };
}
```

---

## SnakeGame::initialState()

Score ergänzen:

```cpp id="fruit38"
return {
  .snake = snake,
  .direction = Direction::Down,
  .fruits = fruits,
  .score = 0
};
```

---

## SnakeUpdate::tick()

Beim Essen werden die Punkte der jeweiligen Frucht zum Score addiert:

```cpp id="fruit39"
if (fruit != state.fruits.end()) {
  state.score +=
    score(fruit->type);

  state.fruits.erase(fruit);

  event(DidEatFruit{});
} else {
  shrinkSnake(snake);
}
```

Die bestehende `spawnFruit()`-Logik bleibt unverändert.

---

# SnakeRender.cpp

## Farben

Drei Farben definieren:

```cpp id="fruit310"
const Color APPLE_COLOR = {
  .r = 255,
  .g = 0,
  .b = 0
};

const Color BERRY_COLOR = {
  .r = 0,
  .g = 0,
  .b = 255
};

const Color BANANA_COLOR = {
  .r = 255,
  .g = 255,
  .b = 0
};
```

---

## color()

Ordnet jedem Fruchttyp seine Farbe zu:

```cpp id="fruit311"
Color color(FruitType type) {
  switch (type) {
    case FruitType::Apple:
      return APPLE_COLOR;

    case FruitType::Berry:
      return BERRY_COLOR;

    case FruitType::Banana:
      return BANANA_COLOR;
  }

  return APPLE_COLOR;
}
```

---

## drawGame()

Beim Zeichnen bekommt jede Frucht ihre eigene Farbe:

```cpp id="fruit312"
for (const auto& fruit : state.fruits) {
  if (!shouldShow(fruit)) {
    continue;
  }

  matrix.drawPixel(
    fruit.position,
    color(fruit.type)
  );
}
```

Das Blinken aus Aufgabe 2 funktioniert damit unverändert weiter.

---

## SnakeRender::state()

Der Score wird jetzt direkt aus dem State gelesen:

```cpp id="fruit313"
screen.drawNumber(
  state.score
);
```

---

## SnakeRender::gameOver()

Auch der Game-Over-Screen verwendet den neuen Score:

```cpp id="fruit314"
screen.drawGameOver(
  state.score
);
```

Der bisherige Helper

```cpp id="fruit315"
uint32_t score(const SnakeState& state)
```

kann gelöscht werden.

---

# Ergebnis

Unsere Frucht hat sich über die drei Aufgaben schrittweise entwickelt:

```cpp id="fruit316"
// Ausgangspunkt
Position fruit;
```

↓

```cpp id="fruit317"
// Aufgabe 1
std::vector<Position> fruits;
```

↓

```cpp id="fruit318"
// Aufgabe 2
struct Fruit {
  Position position;
  uint32_t lifetime;
};

std::vector<Fruit> fruits;
```

↓

```cpp id="fruit319"
// Aufgabe 3
enum class FruitType {
  Apple,
  Berry,
  Banana
};

struct Fruit {
  Position position;
  FruitType type;
  uint32_t lifetime;
};

std::vector<Fruit> fruits;
```

Damit baut jede Aufgabe direkt auf der vorherigen auf, ohne dass wir die Architektur des Spiels grundsätzlich verändern müssen.
