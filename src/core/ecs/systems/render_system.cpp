#include "render_system.h"
#include <stdio.h>

void RenderSystem::Init() {
    printf("RenderSystem initialized\n");
    cameraX = 0.0f;
    cameraY = 0.0f;
}

void RenderSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    // First render regular sprites
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_SPRITE) &&
            !entities->HasComponent(entity, COMPONENT_ANIMATION)) {  // Only if not animated
            
            TransformComponent* transform = 
                (TransformComponent*)components->GetComponentData(entity, COMPONENT_TRANSFORM);
            SpriteComponent* sprite = 
                (SpriteComponent*)components->GetComponentData(entity, COMPONENT_SPRITE);
            
            if (transform && sprite && sprite->texture) {
                RenderEntity(transform, sprite);
            }
        }
    }

    // Then handle animated entities
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_ANIMATION)) {
            TransformComponent* transform = 
                (TransformComponent*)components->GetComponentData(entity, COMPONENT_TRANSFORM);
            AnimationComponent* anim = 
                (AnimationComponent*)components->GetComponentData(entity, COMPONENT_ANIMATION);
            
            if (transform && anim && anim->spriteSheet) {
                // Update animation
                if (anim->playing) {
                    anim->accumulator += deltaTime;
                    if (anim->accumulator >= anim->frameTime) {
                        anim->accumulator -= anim->frameTime;
                        
                        anim->currentFrame++;
                        if (anim->currentFrame >= anim->totalFrames) {
                            if (anim->loop) {
                                anim->currentFrame = 0;
                            } else {
                                anim->currentFrame = anim->totalFrames - 1;
                                anim->playing = false;
                            }
                        }
                        
                        // Update frame rectangle
                        int row = anim->currentFrame / anim->columns;
                        int col = anim->currentFrame % anim->columns;
                        anim->frameRect.x = col * anim->frameWidth;
                        anim->frameRect.y = row * anim->frameHeight;
                    }
                }

                // Render the current frame
                RenderAnimatedEntity(transform, anim);
            }
        }
    }
}

void RenderSystem::RenderEntity(TransformComponent* transform, SpriteComponent* sprite) {
    if (!sprite->texture || !sprite->texture->sdlTexture) return;

    // Calculate screen position (applying camera offset)
    int screenX = (int)(transform->x - cameraX);
    int screenY = (int)(transform->y - cameraY);
    
    // Create destination rectangle
    SDL_Rect destRect;
    destRect.x = screenX;
    destRect.y = screenY;
    destRect.w = sprite->width * transform->scale;
    destRect.h = sprite->height * transform->scale;
    
    // Create source rectangle (for sprite sheets)
    SDL_Rect* srcRect = (sprite->srcRect.w > 0) ? &sprite->srcRect : NULL;
    
    // Calculate rotation center
    SDL_Point center = {
        (int)(sprite->width * transform->scale / 2),
        (int)(sprite->height * transform->scale / 2)
    };
    
    // Render the sprite
    SDL_RenderCopyEx(
        g_Engine.window->renderer,
        sprite->texture->sdlTexture,
        srcRect,
        &destRect,
        transform->rotation,
        &center,
        SDL_FLIP_NONE
    );
}

void RenderSystem::RenderAnimatedEntity(TransformComponent* transform, AnimationComponent* anim) {
    if (!anim->spriteSheet || !anim->spriteSheet->sdlTexture) return;

    // Calculate screen position (applying camera offset)
    int screenX = (int)(transform->x - cameraX);
    int screenY = (int)(transform->y - cameraY);
    
    // Create destination rectangle
    SDL_Rect destRect;
    destRect.x = screenX;
    destRect.y = screenY;
    destRect.w = anim->frameWidth * transform->scale;
    destRect.h = anim->frameHeight * transform->scale;
    
    // Calculate rotation center
    SDL_Point center = {
        (int)(anim->frameWidth * transform->scale / 2),
        (int)(anim->frameHeight * transform->scale / 2)
    };
    
    // Render the sprite
    SDL_RenderCopyEx(
        g_Engine.window->renderer,
        anim->spriteSheet->sdlTexture,
        &anim->frameRect,
        &destRect,
        transform->rotation,
        &center,
        SDL_FLIP_NONE
    );
}

void RenderSystem::Destroy() {
    printf("RenderSystem destroyed\n");
} 