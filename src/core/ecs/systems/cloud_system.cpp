#include "cloud_system.h"
#include <stdio.h>

void CloudSystem::Init() {
    printf("CloudSystem initialized\n");
}

void CloudSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    // Find squirrel entity first
    EntityID squirrelEntity = 0;
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_SQUIRREL)) {
            squirrelEntity = entity;
            break;
        }
    }
    
    if (!squirrelEntity) return;
    
    TransformComponent* squirrelTransform = &components->transforms[squirrelEntity];
    SquirrelComponent* squirrel = &components->squirrelComponents[squirrelEntity];
    
    // Check all clouds
    for (EntityID cloudEntity = 1; cloudEntity < MAX_ENTITIES; cloudEntity++) {
        if (!entities->HasComponent(cloudEntity, COMPONENT_CLOUD)) continue;
        
        CloudComponent* cloud = &components->clouds[cloudEntity];
        TransformComponent* cloudTransform = &components->transforms[cloudEntity];
        SpriteComponent* cloudSprite = &components->sprites[cloudEntity];
        
        // Calculate cloud boundaries
        float cloudTop = cloudTransform->y + COLLISION_GRACE_DISTANCE;
        float cloudBottom = cloudTransform->y + cloudSprite->height - COLLISION_GRACE_DISTANCE;
        float cloudLeft = cloudTransform->x + COLLISION_GRACE_DISTANCE;
        float cloudRight = cloudTransform->x + cloudSprite->width - COLLISION_GRACE_DISTANCE;
        
        // Calculate squirrel boundaries
        SpriteComponent* squirrelSprite = &components->sprites[squirrelEntity];
        float squirrelTop = squirrelTransform->y;
        float squirrelBottom = squirrelTransform->y + squirrelSprite->height;
        float squirrelLeft = squirrelTransform->x;
        float squirrelRight = squirrelTransform->x + squirrelSprite->width;
        
        // Check for collision
        if (squirrelRight > cloudLeft && squirrelLeft < cloudRight &&
            squirrelBottom > cloudTop && squirrelTop < cloudBottom) {
            
            // Different behavior based on cloud type
            if (cloud->type == CLOUD_WHITE && !cloud->wasHit) {
                // Set cloud as hit so it can't be used again
                cloud->wasHit = true;
                
                // Put squirrel in wiggling state regardless of direction
                squirrel->state = SQUIRREL_STATE_WIGGLING;
                
                // Optional: You might want to add some visual feedback
                // cloudSprite->alpha = 0.5f; // If you have alpha support
            }
            else if (cloud->type == CLOUD_BLACK) {
                // Bounce effect
                squirrel->velocityY = -cloud->bounceForce;
            }
        }
    }
}

void CloudSystem::Destroy() {
    printf("CloudSystem destroyed\n");
} 