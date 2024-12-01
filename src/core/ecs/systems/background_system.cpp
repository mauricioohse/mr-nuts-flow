#include "background_system.h"
#include "../components.h"
#include "../../engine.h"
#include "../../window.h"
#include "../../../game/game.h"

void BackgroundSystem::Init() {
    printf("BackgroundSystem initialized\n");
}

void BackgroundSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    // Get camera position first
    CameraComponent* camera = nullptr;
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_CAMERA)) {
            camera = &components->cameras[entity];
            break;
        }
    }
    
    if (!camera) return;

    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_BACKGROUND | COMPONENT_TRANSFORM | COMPONENT_SPRITE)) {
            BackgroundComponent* background = &components->backgrounds[entity];
            TransformComponent* transform = &components->transforms[entity];
            SpriteComponent* sprite = &components->sprites[entity];
            
            // Update X position based on camera with parallax
            transform->x = -camera->x * background->parallaxFactor - 500;
            
            // Check if this is the bottom background (single image)
            bool isBottomBackground = transform->y >= GAME_HEIGHT - WINDOW_HEIGHT;
            
            if (isBottomBackground) {
                
                // Only render if camera is near the bottom
                if (camera->y + camera->viewportHeight > GAME_HEIGHT - WINDOW_HEIGHT) {
                    // For bottom background, we want it fixed at the bottom of the game
                    // but still slightly affected by parallax
                    float yPos = GAME_HEIGHT - WINDOW_HEIGHT - camera->y;
                    
                    TransformComponent *squirrelTransf = 
                        (TransformComponent*)g_Engine.componentArrays.GetComponentData(g_Game.squirrelEntity, COMPONENT_SQUIRREL);

                    SDL_Rect destRect = {
                        (int)(squirrelTransf->x), // follows the squirrel X
                        (int)yPos + WINDOW_HEIGHT,
                        sprite->width,
                        sprite->height
                    };
                    SDL_RenderCopy(g_Engine.window->renderer, sprite->texture->sdlTexture, NULL, &destRect);
                }
            } else {
                // Regular repeating background logic
                for (int i = 0; i < background->repeatCount; i++) {
                    float yPos = i * sprite->height - camera->y * background->parallaxFactor;
                    SDL_Rect destRect = {
                        (int)transform->x,
                        (int)yPos,
                        sprite->width,
                        sprite->height
                    };
                    SDL_RenderCopy(g_Engine.window->renderer, sprite->texture->sdlTexture, NULL, &destRect);
                }
            }
        }
    }
}

void BackgroundSystem::Destroy() {
    printf("BackgroundSystem destroyed\n");
} 