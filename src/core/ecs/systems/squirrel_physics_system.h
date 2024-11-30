#ifndef SQUIRREL_PHYSICS_SYSTEM_H
#define SQUIRREL_PHYSICS_SYSTEM_H

#include "../systems.h"
#include "../entity.h"
#include "../components.h"
#include "../components/squirrel_components.h"

struct SquirrelPhysicsSystem : System {
    void Init() override;
    void Destroy() override;
    void Update(float deltaTime, EntityManager* entities, ComponentArrays* components) override;
private:
    void UpdateVelocity(SquirrelComponent* squirrel, float deltaTime);
    void ApplyMaxSpeed(SquirrelComponent* squirrel);
};

#endif 