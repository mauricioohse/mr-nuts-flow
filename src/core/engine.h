#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "resource_manager.h"
#include "ecs/systems.h"
#include "ecs/components.h"
#include "ecs/entity.h"
#include "ecs/entity_test.h"
// #include "window.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// Core engine constants
#define TARGET_FPS 60
#define FRAME_TIME (1000.0f / TARGET_FPS)

// Forward declarations
struct Window;
struct Entity;
struct ResourceManager;
class Game;

// Core engine systems
struct Engine {
    bool isRunning;
    float deltaTime;
    Uint32 lastFrameTime;
    
    // Core systems
    Window* window;
    ResourceManager* resources;
    EntityManager entityManager;
    SystemManager systemManager;
    ComponentArrays componentArrays;
    
    // Initialize the engine
    static bool Init();
    
    // Cleanup the engine
    static void Cleanup();

    void RunFrame();

    // Main loop
    void Run();
};

extern Engine g_Engine;  // Global engine instance
