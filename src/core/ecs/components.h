#pragma once
#include "ecs_types.h"
#include "../resource_manager.h"
#include "components/squirrel_components.h"
#include "string.h"
#include "stdio.h"
#include "base_component.h"


struct TransformComponent : Component {
    float x, y;
    float rotation;
    float scale;

    void Init(float posX, float posY, float rot = 0.0f, float scl = 1.0f) {
        x = posX;
        y = posY;
        rotation = rot;
        scale = scl;
    }

    void Destroy() override {
        x = 0.0f;
        y = 0.0f;
        rotation = 0.0f;
        scale = 1.0f;
    }
};

struct SpriteComponent : Component {
    Texture* texture;
    int width, height;
    SDL_Rect srcRect;

    void Init(Texture* tex) {
        texture = tex;
        if (texture) {
            width = texture->width;
            height = texture->height;
            srcRect = {0, 0, width, height};
        } else {
            texture = nullptr;
            width = 0;
            height = 0;
            srcRect = {0, 0, 0, 0};
        }
    }

    void ChangeTexture(Texture* newTexture) {
        texture = newTexture;
        if (texture) {
            width = texture->width;
            height = texture->height;
            srcRect = {0, 0, width, height};
        }
    }

    void Destroy() override {
        // Note: We don't destroy the texture here as it's managed by ResourceManager
        texture = nullptr;
        width = 0;
        height = 0;
        srcRect = {0, 0, 0, 0};
    }
};

struct WASDControllerComponent : Component {
    float moveSpeed;
    bool canMove;

    void Init(float speed = 200.0f, bool enabled = true) {
        moveSpeed = speed;
        canMove = enabled;
    }

    void Destroy() override {
        moveSpeed = 0.0f;
        canMove = false;
    }
};

struct ColliderComponent : Component {
    float width;
    float height;
    bool isTrigger;  // If true, detects collision but doesn't prevent movement
    bool isStatic;   // If true, this object won't be moved during collision resolution
    
    void Init(float w, float h, bool staticCollider = false, bool triggerCollider = false) {
        width = w;
        height = h;
        isStatic = staticCollider;
        isTrigger = triggerCollider;
    }
    
    void Destroy() override {
        width = 0.0f;
        height = 0.0f;
        isTrigger = false;
        isStatic = false;
    }
};

struct AnimationComponent : Component {
    Texture* spriteSheet;            // The sprite sheet texture
    SDL_Rect frameRect;             // Current frame rectangle
    int frameWidth;                 // Width of each frame
    int frameHeight;                // Height of each frame
    int columns;                    // Number of columns in sheet
    int totalFrames;                // Total number of frames
    
    int currentFrame;               // Current frame index
    float frameTime;                // Time per frame (in seconds)
    float accumulator;              // Time accumulator for animation
    bool playing;                   // Is animation playing?
    bool loop;                      // Should animation loop?

    void Init(Texture* sheet, int frameW, int frameH, int cols, int frames, 
              float time = 0.1f, bool shouldLoop = true) {
        spriteSheet = sheet;
        frameWidth = frameW;
        frameHeight = frameH;
        columns = cols;
        totalFrames = frames;
        
        currentFrame = 0;
        frameTime = time;
        accumulator = 0.0f;
        playing = true;
        loop = shouldLoop;

        // Initialize first frame rectangle
        UpdateFrameRect();
    }

    void Destroy() override {
        spriteSheet = nullptr;
        frameRect = {0, 0, 0, 0};
        frameWidth = 0;
        frameHeight = 0;
        columns = 0;
        totalFrames = 0;
        currentFrame = 0;
        frameTime = 0.0f;
        accumulator = 0.0f;
        playing = false;
        loop = false;
    }

    void UpdateFrameRect() {
        frameRect.x = currentFrame % columns * frameWidth;
        frameRect.y = currentFrame / columns * frameHeight;
        frameRect.w = frameWidth;
        frameRect.h = frameHeight;
    }
};

struct GravityComponent : Component {
    float velocityY;       // Current vertical velocity
    float gravityScale;    // Multiplier for gravity (1.0 = normal, 0.5 = half gravity, etc.)
    bool isGrounded;       // Is the entity touching the ground?
    
    void Init(float scale = 1.0f) {
        velocityY = 0.0f;
        gravityScale = scale;
        isGrounded = false;
    }
    
    void Destroy() override {
        velocityY = 0.0f;
        gravityScale = 1.0f;
        isGrounded = false;
    }
};

// Component initialization functions
void InitTransform(EntityID entity, float x, float y, float rotation = 0.0f, float scale = 1.0f);
void InitSprite(EntityID entity, Texture* texture);
void InitWASDController(EntityID entity, float moveSpeed = 200.0f, bool canMove = true);
void InitCollider(EntityID entity, float width, float height, bool isStatic = false, bool isTrigger = false);
void InitAnimation(EntityID entity, Texture* sheet, int frameW, int frameH, int cols, int frames, 
                   float time = 0.1f, bool shouldLoop = true);
void InitGravity(EntityID entity, float scale = 1.0f);
void InitSquirrel(EntityID entity);
void InitSquirrelPhysics(EntityID entity);

struct ComponentArrays {
    // Component data pools
    TransformComponent transforms[MAX_ENTITIES];
    SpriteComponent sprites[MAX_ENTITIES];
    WASDControllerComponent wasdControllers[MAX_ENTITIES];
    ColliderComponent colliders[MAX_ENTITIES];
    AnimationComponent animations[MAX_ENTITIES];
    GravityComponent gravities[MAX_ENTITIES];
    SquirrelComponent squirrelComponents[MAX_ENTITIES];

    // Core functions
    void* GetComponentData(EntityID entity, ComponentType type);
    void RemoveComponent(EntityID entity, ComponentType type);
    
    // Add this to ComponentArrays struct
    void Init() {
        // Zero out all component arrays
        memset(this, 0, sizeof(ComponentArrays));
        
        printf("ComponentArrays initialized\n");
    }
}; 