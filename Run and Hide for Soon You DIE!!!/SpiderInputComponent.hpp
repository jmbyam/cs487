#pragma once
#include <gamelib.hpp>

class SpiderInputComponent : public GameLib::InputComponent {
public:
    virtual ~SpiderInputComponent() {}

    void update(GameLib::Actor& actor) override;

private:
    float counter_{ 0 };
};
