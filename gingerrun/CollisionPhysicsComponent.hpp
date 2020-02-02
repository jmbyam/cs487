#pragma once
#include <gamelib.hpp>

class CollisionPhysicsComponent : public GameLib::PhysicsComponent {
public:
    virtual ~CollisionPhysicsComponent() {}

    void update(GameLib::Actor& actor, GameLib::World& world) override;

private:
    bool collides(GameLib::Actor& a, GameLib::Actor& b);
    bool pointInside(glm::vec3 p, GameLib::Actor& a);
};
