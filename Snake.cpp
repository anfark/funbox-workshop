#include "Snake.h"

#include <utility>


// -----------------------------------------------------
// Helpers
// -----------------------------------------------------

bool isInsideSnake(
  const Snake& snake,
  Position position
) {
  if (snake.head == position) {
    return true;
  }

  for (const auto& segment : snake.body) {
    if (segment == position) {
      return true;
    }
  }

  return false;
}


bool isSnakeHeadInsideBody(
  const Snake& snake
) {
  for (const auto& segment : snake.body) {
    if (snake.head == segment) {
      return true;
    }
  }

  return false;
}


SnakeState initialState(
  const SnakeConfig& config
) {
  const Position head = {
    .x = config.bounds.w / 2,
    .y = config.bounds.h / 2
  };

  const auto seg0 =
    head + Direction::Up;

  const auto seg1 =
    seg0 + Direction::Left;

  const auto seg2 =
    seg1 + Direction::Down;

  return {
    .snake = {
      .head = head,
      .body = {
        seg0,
        seg1,
        seg2
      }
    },

    .direction = Direction::Down,

    .fruit = {
      .x = 1,
      .y = 1
    }
  };
}


// -----------------------------------------------------
// Update
// -----------------------------------------------------

SnakeUpdate::SnakeUpdate(
  SnakeState& state,
  Emit emit,
  SnakeConfig config
)
  : GameUpdate(
      state,
      std::move(emit)
    ),
    _config(config) {
}


void SnakeUpdate::tick() {
  Snake& snake =
    state.snake;

  // Alten Kopf in den Körper übernehmen.
  snake.body.insert(
    snake.body.begin(),
    snake.head
  );

  // Neuen Kopf berechnen.
  // % sorgt für Wrap-Around am Rand.
  snake.head =
    (snake.head + state.direction)
    % _config.bounds;


  // Selbstkollision.
  if (isSnakeHeadInsideBody(snake)) {
    event(DidEatSnake{});
    return;
  }


  // Frucht gegessen:
  // Das letzte Segment wird NICHT entfernt,
  // dadurch wächst die Schlange.
  if (snake.head == state.fruit) {
    event(DidEatFruit{});

    state.fruit =
      nextFruit();

    return;
  }


  // Normale Bewegung:
  // hinten ein Segment entfernen.
  snake.body.pop_back();
}


void SnakeUpdate::move(
  Direction direction
) {
  if (
    state.direction ==
    inverseDirection(direction)
  ) {
    return;
  }

  state.direction = direction;
}


bool SnakeUpdate::isOver() const {
  return isSnakeHeadInsideBody(
    state.snake
  );
}


Position SnakeUpdate::nextFruit() const {
  const int cellCount =
    _config.bounds.w *
    _config.bounds.h;

  const int current =
    state.fruit.y * _config.bounds.w
    + state.fruit.x;

  // Wir suchen ab der aktuellen Frucht
  // zyklisch das nächste freie Feld.
  //
  // Vorerst absichtlich deterministisch.
  // Randomness können wir später separat ergänzen.
  for (
    int offset = 1;
    offset <= cellCount;
    ++offset
  ) {
    const int index =
      (current + offset)
      % cellCount;

    const Position candidate = {
      .x = index % _config.bounds.w,
      .y = index / _config.bounds.w
    };

    if (
      !isInsideSnake(
        state.snake,
        candidate
      )
    ) {
      return candidate;
    }
  }

  // Sollte nur auftreten, wenn das komplette
  // Spielfeld von der Schlange belegt ist.
  return state.fruit;
}


// -----------------------------------------------------
// Render
// -----------------------------------------------------

SnakeRender::SnakeRender(
  Matrix& matrix,
  Screen& screen,
  Audio& audio
)
  : _matrix(matrix),
    _screen(screen),
    _audio(audio) {
}


void SnakeRender::didChange(
  const SnakeState& state
) {
  // Ein Ton von einem Event darf bis zum nächsten
  // State-Update spielen.
  _audio.stop();


  // -----------------------------
  // Matrix
  // -----------------------------

  auto& matrix =
    _matrix.display();

  matrix.fillScreen(0);


  // Frucht
  const uint16_t fruitColor =
    matrix.Color(
      255,
      0,
      0
    );

  matrix.drawPixel(
    state.fruit.x,
    state.fruit.y,
    fruitColor
  );


  // Körper
  const uint16_t bodyColor =
    matrix.Color(
      0,
      80,
      0
    );

  for (
    const auto& segment :
    state.snake.body
  ) {
    matrix.drawPixel(
      segment.x,
      segment.y,
      bodyColor
    );
  }


  // Kopf
  const uint16_t headColor =
    matrix.Color(
      0,
      255,
      0
    );

  matrix.drawPixel(
    state.snake.head.x,
    state.snake.head.y,
    headColor
  );


  matrix.show();


  // -----------------------------
  // OLED
  // -----------------------------

  auto& screen =
    _screen.display();

  screen.clearDisplay();

  screen.setTextSize(1);
  screen.setTextColor(
    SSD1306_WHITE
  );

  screen.setCursor(
    0,
    0
  );

  screen.println("SNAKE");

  screen.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE
  );

  screen.setCursor(
    0,
    18
  );

  screen.print("Length: ");

  screen.println(
    state.snake.body.size() + 1
  );

  screen.display();
}


void SnakeRender::didTrigger(
  const DidEatFruit&
) {
  _audio.tone(
    880.0f
  );
}


void SnakeRender::didTrigger(
  const DidEatSnake&
) {
  _audio.tone(
    180.0f
  );
}


// -----------------------------------------------------
// Game
// -----------------------------------------------------

SnakeGame::SnakeGame(
  SnakeConfig config,
  Matrix& matrix,
  Screen& screen,
  Audio& audio
)
  : Game(
      initialState(config)
    ),

    _update(
      state(),
      emit(),
      config
    ),

    _render(
      matrix,
      screen,
      audio
    ) {

  setUpdate(_update);
  setRender(_render);
}