#pragma once
#include <gamelib.hpp>

class RandomInputComponent : public GameLib::InputComponent {
public:
    virtual ~RandomInputComponent() {}

    void update(GameLib::Actor& actor) override {
        if (counter_ < GameLib::Context::currentTime_s) {
            counter_ = GameLib::Context::currentTime_s + 1.0f;
            actor.velocity.x = GameLib::random.normal();
            actor.velocity.y = GameLib::random.normal();
            glm::normalize(actor.velocity);
        }
    }

private:
    float counter_{ 0 };
};
