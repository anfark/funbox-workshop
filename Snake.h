#pragma once

#include <variant>
#include <vector>

#include <FunBox.h>


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


class SnakeUpdate : public GameUpdate<SnakeState, SnakeEvent> {
public:
  using GameUpdate::GameUpdate;

  void tick() override;

  void move(Direction direction) override;

  bool isOver() const override;

private:
  Position nextFruit() const;
};


class SnakeRender: public GameRender {
public:
  using GameRender::GameRender;

  void state(const SnakeState& state);

  void event(const DidEatFruit&);

  void event(const DidEatSnake&);

  void gameOver(const SnakeState& state);
};


struct SnakeGame {
  static constexpr const char* NAME = "Snake";

  using State = SnakeState;
  using Event = SnakeEvent;

  using Update = SnakeUpdate;
  using Render =SnakeRender;

  static State initialState();
};