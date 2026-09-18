#pragma once

#include <variant>
#include <vector>

#include <FunBox.h>


struct FroggerState {
  Position frog;

  std::vector<Position> obstacles;

  uint32_t ticks = 0;
};


struct DidHitObstacle {};
struct DidReachGoal {};
struct DidTimeout {};


using FroggerEvent =
  std::variant<
    DidHitObstacle,
    DidReachGoal,
    DidTimeout
  >;


class FroggerUpdate
  : public GameUpdate<
      FroggerState,
      FroggerEvent
    > {

public:
  using GameUpdate::GameUpdate;

  void tick() override;

  void move(
    Direction direction
  ) override;

  bool isOver() const override;

private:
  void moveObstacles();
  void detectEnd();
};


class FroggerRender
  : public GameRender {

public:
  using GameRender::GameRender;

  void state(
    const FroggerState& state
  );

  void event(
    const DidHitObstacle&
  );

  void event(
    const DidReachGoal&
  );

  void event(
    const DidTimeout&
  );

  void gameOver(
    const FroggerState& state
  );
};


struct FroggerGame {
  static constexpr const char* NAME =
    "Frogger";

  static constexpr uint32_t MAX_TICKS =
    80;

  using State =
    FroggerState;

  using Event =
    FroggerEvent;

  using Update =
    FroggerUpdate;

  using Render =
    FroggerRender;


  static State initialState();
};