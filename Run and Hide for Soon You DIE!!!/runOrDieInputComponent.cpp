#include <gamelib_locator.hpp>
#include "runOrDieInputComponent.hpp"

void RunOrDieInputComponent::update(GameLib::Actor& actor) {
    auto axis1 = GameLib::Locator::getInput()->axis1X;
    if (axis1)
        actor.velocity.x = axis1->getAmount();

    if (lastY == actor.position.y) {
        auto axis2 = GameLib::Locator::getInput()->axis1Y;
        if (axis2)
            actor.velocity.y = 2 * axis2->getAmount();
    }

    lastY = actor.position.y;

    if (actor.spriteFlipX && actor.velocity.x > 0)
        actor.spriteFlipX = false;
    if (!actor.spriteFlipX && actor.velocity.x < 0)
        actor.spriteFlipX = true;
}
