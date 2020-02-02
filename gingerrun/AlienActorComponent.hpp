#pragma once
#include <gamelib.hpp>

class AlienActorComponent : public GameLib::ActorComponent {
    virtual ~AlienActorComponent() {}

    void update(GameLib::Actor& actor, GameLib::World& world) override;
};
