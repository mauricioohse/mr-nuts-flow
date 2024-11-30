#ifndef SQUIRREL_PHYSICS_SYSTEM_H
#define SQUIRREL_PHYSICS_SYSTEM_H

#include "../systems.h"
#include "../entity.h"
#include "../components.h"
#include "../components/squirrel_components.h"
#include "../../input.h"

struct SquirrelPhysicsSystem : System {
    void Init() override;
    void Destroy() override;
    void Update(float deltaTime, EntityManager* entities, ComponentArrays* components) override;
    void UpdateRotation(SquirrelComponent *squirrel, TransformComponent *transform, float deltaTime);

private:
    void UpdateVelocity(SquirrelComponent* squirrel, float deltaTime);
    void ApplyMaxSpeed(SquirrelComponent* squirrel);
    void HandleRotationInput(SquirrelComponent* squirrel);
};

#endif 