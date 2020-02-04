#pragma once
#include <gamelib_component.hpp>

class RunOrDieInputComponent : public GameLib::InputComponent {
public:
    virtual ~RunOrDieInputComponent() {}

	void update(GameLib::Actor& actor) override;

private:
    float lastY{ 0 };
};
