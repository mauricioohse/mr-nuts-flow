#include "cloud_system.h"
#include <stdio.h>

void CloudSystem::Init() {
    printf("CloudSystem initialized\n");
    cloudHitSoundID = SOUND_CLOUD_HIT;
    cloudBounceSoundID = SOUND_CLOUD_BOUNCE;
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
            
            if (squirrel->hasShield) {
                printf("protected from cloud!\n");
                continue;
            }

            // Different behavior based on cloud type
            if (cloud->type == CLOUD_WHITE) {
                // Put squirrel in wiggling state regardless of direction
                squirrel->state = SQUIRREL_STATE_WIGGLING;
                
                // Play cloud hit sound
                Sound* hitSound = ResourceManager::GetSound(cloudHitSoundID);
                if (hitSound) {
                    int channel = Mix_PlayChannel(-1, hitSound->sdlChunk, 0);
                    Mix_Volume(channel, MIX_MAX_VOLUME);  // Set to full volume (128)
                }
            }
            else if (cloud->type == CLOUD_BLACK) {
                // Bounce effect
                squirrel->velocityY = -cloud->bounceForce;
                
                // Play bounce sound at higher volume
                Sound* bounceSound = ResourceManager::GetSound(cloudBounceSoundID);
                if (bounceSound) {
                    int channel = Mix_PlayChannel(-1, bounceSound->sdlChunk, 0);
                    Mix_Volume(channel, MIX_MAX_VOLUME);  // Set to full volume (128)
                }
            }
        }
    }
}

void CloudSystem::Destroy() {
    printf("CloudSystem destroyed\n");
} 