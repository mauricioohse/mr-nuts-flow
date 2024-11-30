#include "squirrel_physics_system.h"
#include <math.h>
#include <stdio.h>

void SquirrelPhysicsSystem::Init() {
    // Nothing to initialize
}

void SquirrelPhysicsSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_SQUIRREL)) {
            SquirrelComponent* squirrel = 
                (SquirrelComponent*)components->GetComponentData(entity, COMPONENT_SQUIRREL);
            
            printf("Entity %d squirrel component address: %p, gravity: %f\n", 
                   entity, (void*)squirrel, squirrel->gravity);
            TransformComponent* transform = 
                (TransformComponent*)components->GetComponentData(entity, COMPONENT_TRANSFORM);

            // Update velocity based on gravity and state
            UpdateVelocity(squirrel, deltaTime);
            
            // Apply max speed limits
            ApplyMaxSpeed(squirrel);

            // Update position
            transform->x += squirrel->velocityX * deltaTime;
            transform->y += squirrel->velocityY * deltaTime;

            // printf("squirrel id %d physics updated - pos:(%f,%f) vel:(%f,%f)\n", 
            //        entity, transform->x, transform->y,
            //        squirrel->velocityX, squirrel->velocityY);
        }
    }
}

void SquirrelPhysicsSystem::UpdateVelocity(SquirrelComponent* squirrel, float deltaTime) {
    // Apply gravity
    squirrel->velocityY += squirrel->gravity * deltaTime;

    // printf("Applied gravity: %f, new velocityY: %f\n", squirrel->gravity * deltaTime, squirrel->velocityY);

    // Additional state-specific physics will be added later
}

void SquirrelPhysicsSystem::ApplyMaxSpeed(SquirrelComponent* squirrel) {
    float currentSpeed = sqrtf(squirrel->velocityX * squirrel->velocityX + 
                             squirrel->velocityY * squirrel->velocityY);
    
    if (currentSpeed > squirrel->maxSpeed) {
        float scale = squirrel->maxSpeed / currentSpeed;
        squirrel->velocityX *= scale;
        squirrel->velocityY *= scale;
    }
} 

void SquirrelPhysicsSystem::Destroy(){}
