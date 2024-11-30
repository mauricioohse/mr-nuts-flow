#pragma once
#include "../core/window.h"
#include "../core/resource_manager.h"
#include "../core/ecs/systems/render_system.h"
#include "../core/ecs/systems/wasd_controller_system.h"
#include "../core/ecs/systems/collision_system.h"
#include "../core/ecs/systems/gravity_system.h"
#include "../core/ecs/systems/squirrel_physics_system.h"
#include "../core/ecs/systems/camera_system.h"

class Game {
public:
    bool Init();
    void HandleInput();
    void Update(float deltaTime);
    void Render();
    void Cleanup();

private:
    // Systems
    RenderSystem renderSystem;
    WASDControllerSystem wasdSystem;
    CollisionSystem collisionSystem;
    GravitySystem gravitySystem;
    SquirrelPhysicsSystem squirrelSystem;
    CameraSystem cameraSystem;
    
    // Entities
    EntityID squirrelEntity;
    EntityID cameraEntity;
    
    // Resources (using IDs instead of pointers)
    SoundID hitSoundID;
    FontID fpsFontID;
};

extern Game g_Game;  // Global game instance 