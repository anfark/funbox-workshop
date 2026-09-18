#pragma once

#include <variant>
#include <vector>

#include <FunBox.h>


struct StarFallState {
  int shipX;

  std::vector<Position> stars;
  std::vector<Position> shots;

  uint32_t ticks = 0;
  uint32_t score = 0;
};


struct DidShoot {};


struct DidHitStar {
  Position position;
};


struct DidHitShip {};


using StarFallEvent =
  std::variant<
    DidShoot,
    DidHitStar,
    DidHitShip
  >;


class StarFallUpdate
  : public GameUpdate<
      StarFallState,
      StarFallEvent
    > {

public:
  using GameUpdate::GameUpdate;

  void tick() override;

  void move(
    Direction direction
  ) override;

  void click() override;

  bool isOver() const override;

private:
  void moveStars();
  void moveShots();
  void spawnStar();
  void detectHits();
};


class StarFallRender
  : public GameRender {

public:
  using GameRender::GameRender;

  void state(
    const StarFallState& state
  );

  void event(
    const DidShoot&
  );

  void event(
    const DidHitStar&
  );

  void event(
    const DidHitShip&
  );

  void gameOver(
    const StarFallState& state
  );
};


struct StarFallGame {
  static constexpr const char* NAME =
    "StarFall";

  using State =
    StarFallState;

  using Event =
    StarFallEvent;

  using Update =
    StarFallUpdate;

  using Render =
    StarFallRender;


  static State initialState();
};