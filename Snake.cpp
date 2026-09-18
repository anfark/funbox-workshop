#include "Snake.h"

#include <Arduino.h>
#include <utility>


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


bool isInsideSnake(
  const Snake& snake,
  Position position
) {
  if (
    snake.head ==
    position
  ) {
    return true;
  }

  for (
    const auto& segment :
    snake.body
  ) {
    if (
      segment ==
      position
    ) {
      return true;
    }
  }

  return false;
}


bool isSnakeHeadInsideBody(
  const Snake& snake
) {
  for (
    const auto& segment :
    snake.body
  ) {
    if (
      snake.head ==
      segment
    ) {
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

  snake.body.insert(
    snake.body.begin(),
    snake.head
  );

  snake.head =
    (snake.head + state.direction)
    % _config.bounds;


  if (
    isSnakeHeadInsideBody(
      snake
    )
  ) {
    event(
      DidEatSnake{}
    );

    return;
  }


  if (
    snake.head ==
    state.fruit
  ) {
    event(
      DidEatFruit{}
    );

    state.fruit =
      nextFruit();

    return;
  }


  snake.body.pop_back();
}


void SnakeUpdate::move(
  Direction direction
) {
  if (
    state.direction ==
    inverseDirection(
      direction
    )
  ) {
    return;
  }

  state.direction =
    direction;
}


bool SnakeUpdate::isOver() const {
  return isSnakeHeadInsideBody(
    state.snake
  );
}


Position SnakeUpdate::nextFruit() const {
  Position position;

  do {
    position = {
      .x = random(
        _config.bounds.w
      ),
      .y = random(
        _config.bounds.h
      )
    };
  }
  while (
    isInsideSnake(
      state.snake,
      position
    )
  );

  return position;
}


void SnakeRender::state(
  const SnakeState& state
) {
  matrix.clear();

  matrix.drawPixel(
    state.fruit,
    FRUIT_COLOR
  );

  for (
    const auto& segment :
    state.snake.body
  ) {
    matrix.drawPixel(
      segment,
      BODY_COLOR
    );
  }

  matrix.drawPixel(
    state.snake.head,
    HEAD_COLOR
  );

  matrix.show();


  auto& display =
    screen.display();

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(
    SSD1306_WHITE
  );

  display.setCursor(
    0,
    0
  );

  display.println(
    "SNAKE"
  );

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE
  );

  display.setCursor(
    0,
    18
  );

  display.print(
    "Length: "
  );

  display.println(
    state.snake.body.size() + 1
  );

  display.display();
}


void SnakeRender::event(
  const DidEatFruit&
) {
  audio.effect(
    EAT_SOUND
  );
}


void SnakeRender::event(
  const DidEatSnake&
) {
  audio.effect(
    COLLISION_SOUND
  );
}


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

  connect(
    _update,
    _render
  );
}