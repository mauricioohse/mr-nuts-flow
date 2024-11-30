#include "background_system.h"
#include "../components.h"
#include "../../engine.h"
#include "../../window.h"

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
            transform->x = -camera->x * background->parallaxFactor;
            
            // Render multiple instances vertically
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

void BackgroundSystem::Destroy() {
    printf("BackgroundSystem destroyed\n");
} 