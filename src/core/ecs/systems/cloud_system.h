#pragma once
#include "../systems.h"
#include "../entity.h"
#include "../components.h"

#define COLLISION_GRACE_DISTANCE 10 // 10pxs

struct CloudSystem : System {
    void Init() override;
    void Update(float deltaTime, EntityManager* entities, ComponentArrays* components) override;
    void Destroy() override;

private:
    SoundID cloudHitSoundID;
    SoundID cloudBounceSoundID;
}; 