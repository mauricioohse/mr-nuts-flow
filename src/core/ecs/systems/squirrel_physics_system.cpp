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
            
            TransformComponent* transform = 
                (TransformComponent*)components->GetComponentData(entity, COMPONENT_TRANSFORM);

            // Handle rotation input first
            HandleRotationInput(squirrel);

            // Update velocity based on gravity and state
            UpdateVelocity(squirrel, deltaTime);
            
            // Update rotation
            UpdateRotation(squirrel, transform, deltaTime);
            
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

void SquirrelPhysicsSystem::UpdateRotation(SquirrelComponent* squirrel, TransformComponent* transform, float deltaTime) {
    // Calculate current velocity angle
    float velocityAngle = atan2f(squirrel->velocityY, squirrel->velocityX) * 180.0f / M_PI;
    
    // Smoothly interpolate current rotation to match velocity direction
    float angleDiff = velocityAngle - squirrel->rotation;
    
    // Normalize angle difference to [-180, 180]
    while (angleDiff > 180.0f) angleDiff -= 360.0f;
    while (angleDiff < -180.0f) angleDiff += 360.0f;
    
    // Smoothly rotate towards velocity direction
    squirrel->rotation += angleDiff * deltaTime * SQUIRREL_ROTATION_SPEED;
    
    // Normalize final rotation
    while (squirrel->rotation > 180.0f) squirrel->rotation -= 360.0f;
    while (squirrel->rotation < -180.0f) squirrel->rotation += 360.0f;
    
    // Update transform rotation
    transform->rotation = squirrel->rotation ;//+ SQUIRREL_SPRITE_ROTATION_OFFSET;
}

void SquirrelPhysicsSystem::UpdateVelocity(SquirrelComponent* squirrel, float deltaTime) {
    // Store old velocity for rotation calculation
    float oldVelX = squirrel->velocityX;
    float oldVelY = squirrel->velocityY;
    
    // Apply gravity
    squirrel->velocityY += squirrel->gravity * deltaTime;
    
    // If velocity direction changed significantly, update rotation
    float newAngle = atan2f(squirrel->velocityY, squirrel->velocityX);
    float oldAngle = atan2f(oldVelY, oldVelX);
    if (fabsf(newAngle - oldAngle) > 0.1f) {  // ~5.7 degrees threshold
        squirrel->targetRotation = newAngle * 180.0f / M_PI;
    }

    // Apply state-specific speed limits
    switch(squirrel->state) {
        case SQUIRREL_STATE_OPEN_ARMS:
            squirrel->maxSpeed = SQUIRREL_OPEN_ARMS_MAX_SPEED;
            break;
        case SQUIRREL_STATE_CLOSED_ARMS:
            squirrel->maxSpeed = SQUIRREL_CLOSED_ARMS_MAX_SPEED;
            break;
        case SQUIRREL_STATE_WIGGLING:
            squirrel->maxSpeed = SQUIRREL_WIGGLE_MAX_SPEED;
            break;
    }
}

void SquirrelPhysicsSystem::ApplyMaxSpeed(SquirrelComponent* squirrel) {
    
    float currentSpeed = sqrtf(squirrel->velocityX * squirrel->velocityX + 
                             squirrel->velocityY * squirrel->velocityY);

    if (currentSpeed > squirrel->maxSpeed) {
        // Calculate deceleration factor
        float scale = squirrel->maxSpeed / currentSpeed;
        
        // Apply smooth deceleration
        squirrel->velocityX *= scale;
        squirrel->velocityY *= scale;
    }
} 

void SquirrelPhysicsSystem::Destroy(){}

void SquirrelPhysicsSystem::HandleRotationInput(SquirrelComponent* squirrel) {
    // Only handle rotation input in OPEN_ARMS state
    if (squirrel->state != SQUIRREL_STATE_OPEN_ARMS) {
        return;
    }

    // Check for left rotation (A or Left Arrow)
    if (Input::IsKeyPressed(SDL_SCANCODE_A) || Input::IsKeyPressed(SDL_SCANCODE_LEFT)) {
        squirrel->targetRotation -= SQUIRREL_TAP_ROTATION;
        
        // Rotate velocity vector
        float angle = -SQUIRREL_TAP_ROTATION * M_PI / 180.0f;
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        float newVelX = squirrel->velocityX * cosAngle - squirrel->velocityY * sinAngle;
        float newVelY = squirrel->velocityX * sinAngle + squirrel->velocityY * cosAngle;
        squirrel->velocityX = newVelX;
        squirrel->velocityY = newVelY;
    }
    
    // Check for right rotation (D or Right Arrow)
    if (Input::IsKeyPressed(SDL_SCANCODE_D) || Input::IsKeyPressed(SDL_SCANCODE_RIGHT)) {
        squirrel->targetRotation += SQUIRREL_TAP_ROTATION;
        
        // Rotate velocity vector
        float angle = SQUIRREL_TAP_ROTATION * M_PI / 180.0f;
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        float newVelX = squirrel->velocityX * cosAngle - squirrel->velocityY * sinAngle;
        float newVelY = squirrel->velocityX * sinAngle + squirrel->velocityY * cosAngle;
        squirrel->velocityX = newVelX;
        squirrel->velocityY = newVelY;
    }
}
