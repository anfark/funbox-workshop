#pragma once

#include <variant>
#include <vector>

#include <FunBox.h>


struct SnakeConfig {
  Size bounds;
};


using SnakeBody =
  std::vector<Position>;


struct Snake {
  Position head;
  SnakeBody body;
};


struct SnakeState {
  Snake snake;
  Direction direction;
  Position fruit;
};


struct DidEatFruit {};
struct DidEatSnake {};


using SnakeEvent =
  std::variant<
    DidEatFruit,
    DidEatSnake
  >;


class SnakeUpdate
  : public GameUpdate<
      SnakeState,
      SnakeEvent
    > {

public:
  SnakeUpdate(
    SnakeState& state,
    Emit emit,
    SnakeConfig config
  );

  void tick() override;

  void move(
    Direction direction
  ) override;

  bool isOver() const override;

private:
  const SnakeConfig _config;

  Position nextFruit() const;
};


class SnakeRender
  : public GameRender {

public:
  using GameRender::GameRender;

  void state(
    const SnakeState& state
  );

  void event(
    const DidEatFruit&
  );

  void event(
    const DidEatSnake&
  );
};


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