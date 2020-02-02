#include "CollisionPhysicsComponent.hpp"

void CollisionPhysicsComponent::update(GameLib::Actor& actor, GameLib::World& world) {
    glm::vec3 prevPosition = actor.position;
    actor.velocity.y += actor.dt * 9.8f;
	SimplePhysicsComponent::update(actor, world);
    if (prevPosition.y == actor.position.y) {
        actor.velocity.y = 0;
	}

    // do collision detection
    for (auto& a : world.actors) {
        if (!a->active)
            continue;
        if (a->getId() == actor.getId())
            continue;
        if (collides(*a, actor)) {
            glm::vec3 nextPosition = actor.position;
            actor.position = prevPosition;
            if (!collides(*a, actor)) {
				// due to the previous position
            }
            //HFLOGDEBUG("boom! between %d and %d", a->getId(), actor.getId());
		}
	}
}

bool CollisionPhysicsComponent::collides(GameLib::Actor& a, GameLib::Actor& b) {
    glm::vec3 amin = a.position;
    glm::vec3 amax = a.position + a.size;
    glm::vec3 bmin = b.position;
    glm::vec3 bmax = b.position + b.size;

    bool overlapX = (amin.x <= bmax.x && amax.x >= bmin.x);
    bool overlapY = (amin.y <= bmax.y && amax.y >= bmin.z);
    bool overlapZ = (amin.z <= bmax.z && amax.z >= bmin.z);
    return overlapX && overlapY && overlapZ;
}

bool CollisionPhysicsComponent::pointInside(glm::vec3 p, GameLib::Actor& a) {
    glm::vec3 amin = a.position;
    glm::vec3 amax = a.position + a.size;
    bool insideX = amin.x >= p.x && amax.x <= p.x;
    bool insideY = amin.y >= p.y && amax.y <= p.y;
    bool insideZ = amin.z >= p.z && amax.z <= p.z;
    return insideX && insideY && insideZ;
}
