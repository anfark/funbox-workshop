#pragma once

#include <variant>
#include <vector>

#include <FunBox.h>


// -----------------------------------------------------
// Config
// -----------------------------------------------------

struct SnakeConfig {
  Size bounds;
};


// -----------------------------------------------------
// State
// -----------------------------------------------------

using SnakeBody = std::vector<Position>;

struct Snake {
  Position head;
  SnakeBody body;
};

struct SnakeState {
  Snake snake;
  Direction direction;
  Position fruit;
};


// -----------------------------------------------------
// Events
// -----------------------------------------------------

struct DidEatFruit {};
struct DidEatSnake {};

using SnakeEvent = std::variant<
  DidEatFruit,
  DidEatSnake
>;


// -----------------------------------------------------
// Update
// -----------------------------------------------------

class SnakeUpdate
  : public GameUpdate<SnakeState, SnakeEvent> {

public:
  SnakeUpdate(
    SnakeState& state,
    Emit emit,
    SnakeConfig config
  );

  void tick() override;
  void move(Direction direction) override;

  bool isOver() const override;

private:
  const SnakeConfig _config;

  Position nextFruit() const;
};


// -----------------------------------------------------
// Render
// -----------------------------------------------------

class SnakeRender
  : public GameRender<
      SnakeRender,
      SnakeState,
      SnakeEvent
    > {

public:
  SnakeRender(
    Matrix& matrix,
    Screen& screen,
    Audio& audio
  );

  void didChange(
    const SnakeState& state
  );

  void didTrigger(
    const DidEatFruit&
  );

  void didTrigger(
    const DidEatSnake&
  );

private:
  Matrix& _matrix;
  Screen& _screen;
  Audio& _audio;
};


// -----------------------------------------------------
// Game
// -----------------------------------------------------

class SnakeGame
  : public Game<
      SnakeState,
      SnakeEvent,
      SnakeUpdate,
      SnakeRender
    > {

public:
  SnakeGame(
    SnakeConfig config,
    Matrix& matrix,
    Screen& screen,
    Audio& audio
  );

private:
  SnakeUpdate _update;
  SnakeRender _render;
};