#include "game.h"
#include "../core/resource_manager.h"
#include "../core/window.h"
#include "../core/input.h"
#include "cloud_init.h"
#include "peanut_init.h"

Game g_Game;

bool Game::Init() {
    // Initialize systems
    renderSystem.Init();
    squirrelSystem.Init();
    cameraSystem.Init();
    cloudSystem.Init();
    backgroundSystem.Init();
    peanutSystem.Init();
    collisionSystem.Init();
    
    
    g_Engine.systemManager.RegisterSystem(&backgroundSystem);
    g_Engine.systemManager.RegisterSystem(&renderSystem);
    g_Engine.systemManager.RegisterSystem(&squirrelSystem);
    g_Engine.systemManager.RegisterSystem(&cameraSystem);
    g_Engine.systemManager.RegisterSystem(&cloudSystem);
    g_Engine.systemManager.RegisterSystem(&peanutSystem);
    g_Engine.systemManager.RegisterSystem(&collisionSystem);

    // Create background
    backgroundEntity = g_Engine.entityManager.CreateEntity();
    Texture* backgroundTexture = ResourceManager::GetTexture(TEXTURE_BACKGROUND_MIDDLE);
    ADD_TRANSFORM(backgroundEntity, 0.0f, 0.0f, 0.0f, 1.0f);
    ADD_SPRITE(backgroundEntity, backgroundTexture);
    ADD_BACKGROUND(backgroundEntity, 0.5f);  // 0.5 parallax factor for medium depth

    // Create bottom background
    bottomBackgroundEntity = g_Engine.entityManager.CreateEntity();
    Texture* bottomTexture = ResourceManager::GetTexture(TEXTURE_BACKGROUND_BOTTOM);
    ADD_TRANSFORM(bottomBackgroundEntity, 800.0f, GAME_HEIGHT , 0.0f, 1.0f);
    ADD_SPRITE(bottomBackgroundEntity, bottomTexture);
    ADD_BACKGROUND(bottomBackgroundEntity, 0.5f);

    EntityID Wall_left = g_Engine.entityManager.CreateEntity();
    Texture* spriteTex = ResourceManager::GetTexture(TEXTURE_WALL);
    ADD_TRANSFORM(Wall_left, 0, 0, 0.0f, 1.0f);
    ADD_COLLIDER(Wall_left, 50, GAME_HEIGHT, true, false);
    ADD_SPRITE(Wall_left, spriteTex);
    SpriteComponent *wall_sprite = &g_Engine.componentArrays.sprites[Wall_left];
    wall_sprite->width = 32;
    wall_sprite->height= GAME_HEIGHT;

    EntityID Wall_right = g_Engine.entityManager.CreateEntity();
    ADD_TRANSFORM(Wall_right, 2400, 0, 0.0f, 1.0f);
    ADD_COLLIDER(Wall_right, 50, GAME_HEIGHT, true, false);
    ADD_SPRITE(Wall_right, spriteTex);
    SpriteComponent *wall_right_sprite = &g_Engine.componentArrays.sprites[Wall_right];
    wall_right_sprite->width = 32;
    wall_right_sprite->height= GAME_HEIGHT;

    // Create helicopter entity
    helicopterEntity = g_Engine.entityManager.CreateEntity();
    Texture* helicopterTexture = ResourceManager::GetTexture(TEXTURE_HELICOPTER);
    ADD_TRANSFORM(helicopterEntity, 1200.0f, 100.0f, 0.0f, 1.0f);  // Position above squirrel
    ADD_SPRITE(helicopterEntity, helicopterTexture);

    // Create squirrel entity
    squirrelEntity = g_Engine.entityManager.CreateEntity();
    
    Texture* squirrelTexture = ResourceManager::GetTexture(TEXTURE_SQUIRREL_OPEN);    
    // Add basic components
    ADD_TRANSFORM(squirrelEntity, 1200.0f, 100.0f, 0.0f, 1.0f);  // Center-top of screen
    ADD_SQUIRREL(squirrelEntity);
    ADD_SPRITE(squirrelEntity, squirrelTexture);
    ADD_COLLIDER(squirrelEntity, 32, 32, 0, 0);

    // create camera
    cameraEntity = g_Engine.entityManager.CreateEntity();
    ADD_TRANSFORM(cameraEntity, 1200.0f, 100.0f, 0.0f, 1.0f);
    ADD_CAMERA(cameraEntity, WINDOW_WIDTH, WINDOW_HEIGHT, squirrelEntity);

    Reset();

    // Create manual clouds
    CreateCloudsFromData(cloudList, sizeof(cloudList) / sizeof(CloudInitData));
    
    float cloudSpawnThreshold = 500; 
    GenerateRandomClouds(cloudSpawnThreshold);

    GenerateRandomPeanuts(500.0f);  // Use same threshold as clouds

    // Store IDs for later use
    hitSoundID = SOUND_HIT;
    fpsFontID = FONT_FPS;
    

    gameTimer = 0.0f;


    // Position squirrel below helicopter
    TransformComponent* heliTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(helicopterEntity, COMPONENT_TRANSFORM);
    SpriteComponent* heliSprite = 
        (SpriteComponent*)g_Engine.componentArrays.GetComponentData(helicopterEntity, COMPONENT_SPRITE);
    
    // Adjust squirrel starting position to be just below helicopter
    TransformComponent* squirrelTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_TRANSFORM);
    squirrelTransform->x = heliTransform->x;
    squirrelTransform->y = heliTransform->y + 30;

    gameState = GAME_STATE_PLAYING;
    bestTime = 999999.0f;  // Some high number
    isNewRecord = false;

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
    
    // Add reset on 'R' key press
    if (Input::IsKeyPressed(SDL_SCANCODE_R)) {
        Reset();
    }
}

void Game::Update(float deltaTime) {
    HandleInput();

    // Get squirrel position
    SquirrelComponent *squirrel =
        (SquirrelComponent *)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_SQUIRREL);


    if (gameState == GAME_STATE_PLAYING && squirrel->state != SQUIRREL_STATE_DROPPING) {
        gameTimer += deltaTime;

        // Get squirrel position
        TransformComponent *squirrelTransform =
            (TransformComponent *)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_TRANSFORM);

        // Check if squirrel reached bottom
        if (squirrelTransform->y >= GAME_HEIGHT + 300) {  // Leave some margin at bottom
            gameState = GAME_STATE_FINISHED;
            
            // Check if this is a new record
            if (gameTimer < bestTime) {
                bestTime = gameTimer;
                isNewRecord = true;
            }
        }
    }
}

void Game::Render() {
    // Systems will handle rendering of entities
    g_Engine.systemManager.UpdateSystems(g_Engine.deltaTime, &g_Engine.entityManager, &g_Engine.componentArrays);
    
    // Get squirrel position for height calculation
    TransformComponent* squirrelTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_TRANSFORM);
    SquirrelComponent *squirrel =
        (SquirrelComponent *)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_SQUIRREL);

    // Calculate remaining height (in hundreds of pixels)
    float remainingHeight = (GAME_HEIGHT - squirrelTransform->y) / 100.0f;
    
    // Render FPS counter, timer, height and speed
    char fpsText[32];
    char timerText[32];
    char heightText[32];
    char speedText[32];
    char posText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.1f", 1.0f / g_Engine.deltaTime);
    snprintf(timerText, sizeof(timerText), "Time: %.2f", gameTimer);
    snprintf(heightText, sizeof(heightText), "Height: %.0f", remainingHeight);
    snprintf(speedText, sizeof(speedText), "Max speed: %.0f", squirrel->maxSpeed);
    snprintf(posText, sizeof(posText), "Pos: %.0f, %.0f", squirrelTransform->x, squirrelTransform->y);

    SDL_Color textColor = {255, 255, 255, 255};  // White color
    Font* fpsFont = ResourceManager::GetFont(fpsFontID);
    if (fpsFont) {
        // Render FPS at top right
        ResourceManager::RenderTextAlignedTopRight(fpsFont, fpsText, textColor, 
            g_Engine.window->width - 10, 10);
        // Render timer below FPS
        ResourceManager::RenderTextAlignedTopRight(fpsFont, timerText, textColor, 
            g_Engine.window->width - 10, 30);
        // Render height below timer
        ResourceManager::RenderTextAlignedTopRight(fpsFont, heightText, textColor, 
            g_Engine.window->width - 10, 50);
        // Render speed below height
        ResourceManager::RenderTextAlignedTopRight(fpsFont, speedText, textColor, 
            g_Engine.window->width - 10, 70);
        // Render position below speed
        ResourceManager::RenderTextAlignedTopRight(fpsFont, posText, textColor, 
            g_Engine.window->width - 10, 90);
        
        // If game is finished, show completion message
        if (gameState == GAME_STATE_FINISHED) {
            char finishText[64];
            snprintf(finishText, sizeof(finishText), "FINISHED! Time: %.2f", gameTimer);

            SquirrelComponent *squirrel =
                (SquirrelComponent *)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_SQUIRREL);

            squirrel->state = SQUIRREL_STATE_DROPPING;

            // Center the text
            ResourceManager::RenderTextAlignedCenter(fpsFont, finishText, textColor,
                g_Engine.window->width/2, g_Engine.window->height/2);
                
            if (isNewRecord) {
                ResourceManager::RenderTextAlignedCenter(fpsFont, "NEW RECORD!", textColor,
                    g_Engine.window->width/2, g_Engine.window->height/2 + 30);
            }
        }
    }
}

void Game::Cleanup() {
    // Cleanup entities
    g_Engine.entityManager.DestroyEntity(squirrelEntity);
    
    // Resources will be cleaned up by ResourceManager
}

void Game::Reset() {
    // Reset game state
    gameState = GAME_STATE_PLAYING;
    gameTimer = 0.0f;
    isNewRecord = false;
    
    // Reset helicopter position
    TransformComponent* heliTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(helicopterEntity, COMPONENT_TRANSFORM);
    heliTransform->x = 1200.0f;
    heliTransform->y = 100.0f;
    
    // Reset squirrel position and state  
    TransformComponent* squirrelTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_TRANSFORM);
    SquirrelComponent* squirrel = 
        (SquirrelComponent*)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_SQUIRREL);
        
    
    // Position squirrel below helicopter
    squirrelTransform->x = heliTransform->x;
    squirrelTransform->y = heliTransform->y + 30;
    
    // Reset squirrel state
    squirrel->state = SQUIRREL_STATE_DROPPING;
    squirrel->velocityX = 0;
    squirrel->velocityY = 0;
    squirrel->rotation = 0;
    
    // Reset all peanuts
    MakeAllPeanutsVisibleAgain();

    // resets squirrel stats
    squirrel->Init();
}
