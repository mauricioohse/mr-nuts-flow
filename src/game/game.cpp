#include "game.h"
#include "../core/resource_manager.h"
#include "../core/window.h"
#include "../core/input.h"
#include "cloud_init.h"

Game g_Game;

bool Game::Init() {
    // Initialize systems
    renderSystem.Init();
    squirrelSystem.Init();
    cameraSystem.Init();
    cloudSystem.Init();
    g_Engine.systemManager.RegisterSystem(&renderSystem);
    g_Engine.systemManager.RegisterSystem(&squirrelSystem);
    g_Engine.systemManager.RegisterSystem(&cameraSystem);
    g_Engine.systemManager.RegisterSystem(&cloudSystem);


    // Create squirrel entity
    squirrelEntity = g_Engine.entityManager.CreateEntity();
    
    Texture* squirrelTexture = ResourceManager::GetTexture(TEXTURE_SQUIRREL_OPEN);    
    // Add basic components
    ADD_TRANSFORM(squirrelEntity, 400.0f, 100.0f, 0.0f, 1.0f);  // Center-top of screen
    ADD_SQUIRREL(squirrelEntity);
    ADD_SPRITE(squirrelEntity, squirrelTexture);

    // create camera
    cameraEntity = g_Engine.entityManager.CreateEntity();
    ADD_TRANSFORM(cameraEntity, 400.0f, 100.0f, 0.0f, 1.0f);
    ADD_CAMERA(cameraEntity, WINDOW_WIDTH, WINDOW_HEIGHT, squirrelEntity);

    // Create all clouds at once
    CreateCloudsFromData(cloudList, sizeof(cloudList) / sizeof(CloudInitData));

    // Store IDs for later use
    hitSoundID = SOUND_HIT;
    fpsFontID = FONT_FPS;
    
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
    g_Engine.entityManager.DestroyEntity(squirrelEntity);
    
    // Resources will be cleaned up by ResourceManager
}
