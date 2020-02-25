//#include "ShadowActorComponent.hpp"
//
//void ShadowActorComponent::update(GameLib::Actor& actor, GameLib::World& world) {
//    // do actor stuff here
//    float t = GameLib::Context::currentTime_s;
//    int frame = (int)t;
//
//    actor.velocity.y += actor.dt * 9.0;
//
//	if (frame % 15 == 0) {
//        animTime = t + 1;
//        state = State::WalkingL;
//    }
//
//    if (frame % 15 == 5) {
//        animTime = t + 1;
//        state = State::WalkingR;
//    }
//
//	if (animTime > t) {
//        frame = GameLib::clamp<int>(8 - (int)(8 * (animTime - t)), 0, 7);
//    } else {
//        state = State::StandingL;
//        frame = int(8 * t) % 8;
//    }
//
//    switch (state) {
//    case State::StandingL:
//        actor.spriteId = 19;
//        break;
//    case State::StandingR:
//        actor.spriteId = 20;
//        break;
//    case State::WalkingL:
//        actor.spriteId = 1 + frame;
//        break;
//    case State::WalkingR:
//        actor.spriteId = 10 + frame;
//        break;
//    }
//}
