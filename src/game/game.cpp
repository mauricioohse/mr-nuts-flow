#include "game.h"
#include "../core/resource_manager.h"
#include "../core/window.h"
#include "../core/input.h"

Game g_Game;

bool Game::Init() {
    // Initialize systems
    renderSystem.Init();
    wasdSystem.Init();
    collisionSystem.Init();
    gravitySystem.Init();
    g_Engine.systemManager.RegisterSystem(&renderSystem);
    g_Engine.systemManager.RegisterSystem(&wasdSystem);
    g_Engine.systemManager.RegisterSystem(&collisionSystem);
    g_Engine.systemManager.RegisterSystem(&gravitySystem);



    // Store IDs for later use
    hitSoundID = SOUND_HIT;
    fpsFontID = FONT_FPS;

    // Create box entity
    boxEntity = g_Engine.entityManager.CreateEntity();
    
    Texture* boxTexture = ResourceManager::GetTexture(TEXTURE_BOX_ANIM_SHEET);
    
    // Add and initialize components
    ADD_TRANSFORM(boxEntity, 10.0f, 10.0f, 0.0f, 1.0f);
    ADD_ANIMATION(boxEntity, boxTexture, 32, 32, 4, 4, 0.2f, true);
    ADD_WASD_CONTROLLER(boxEntity, 200.0f, true);
    ADD_COLLIDER(boxEntity, 32, 32, false, false);
    ADD_GRAVITY(boxEntity, 1.0f);
    
    // Create wall entity
    EntityID wallEntity = g_Engine.entityManager.CreateEntity();
    
    Texture* wallTexture = ResourceManager::GetTexture(TEXTURE_WALL);
    
    ADD_TRANSFORM(wallEntity, 100.0f, 100.0f, 0.0f, 1.0f);
    ADD_SPRITE(wallEntity, wallTexture);
    ADD_COLLIDER(wallEntity, wallTexture->width, wallTexture->height, true, false);

    return true;
}

// logic related to inputs on the game should go here
void Game::HandleInput(){

    // Play sound on mouse click
    if (Input::mouseButtonsPressed[0]) {  // Left click
        Sound* hitSound = ResourceManager::GetSound(hitSoundID);
        if (hitSound) {
            Mix_PlayChannel(-1, hitSound->sdlChunk, 0);
        }
    }
}

void Game::Update(float deltaTime) {
    HandleInput();
}

void Game::Render() {
    // Systems will handle rendering of entities
    g_Engine.systemManager.UpdateSystems(g_Engine.deltaTime, &g_Engine.entityManager, &g_Engine.componentArrays);
    
    // Render FPS counter
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", 1.0f / g_Engine.deltaTime);
    
    SDL_Color textColor = {255, 255, 255, 255};  // White color
    Font* fpsFont = ResourceManager::GetFont(fpsFontID);
    if (fpsFont) {
        ResourceManager::RenderTextAlignedTopRight(fpsFont, fpsText, textColor, 
            g_Engine.window->width - 10, 10);
    }
}

void Game::Cleanup() {
    // Cleanup entities
    g_Engine.entityManager.DestroyEntity(boxEntity);
    
    // Resources will be cleaned up by ResourceManager
}
