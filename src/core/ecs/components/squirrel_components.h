#ifndef SQUIRREL_COMPONENTS_H
#define SQUIRREL_COMPONENTS_H

#include "../base_component.h"
#include <stdio.h>
 
typedef enum {
    SQUIRREL_STATE_OPEN_ARMS,
    SQUIRREL_STATE_CLOSED_ARMS,
    SQUIRREL_STATE_WIGGLING
} SquirrelState;

// Physics constants
#define SQUIRREL_GRAVITY 400.0f          // Increased for more noticeable falling
#define SQUIRREL_OPEN_ARMS_MAX_SPEED 300.0f
#define SQUIRREL_CLOSED_ARMS_MAX_SPEED 800.0f
#define SQUIRREL_WIGGLE_MAX_SPEED 100.0f
#define SQUIRREL_ROTATION_SPEED 90.0f  // Degrees per second
#define SQUIRREL_TAP_ROTATION 20.0f     // Degrees per tap
#define SQUIRREL_SPRITE_ROTATION_OFFSET -90.0f  // Rotate sprite 90 degrees to point feet down
#define SQUIRREL_WIGGLE_DURATION 1.0f // in seconds
#define SQUIRREL_GRACE_PERIOD 3.0f // in seconds

struct SquirrelComponent : Component {
    // Gameplay state
    SquirrelState state;
    float wiggleTimer;      // For wiggle state duration
    float graceTimer;       // For post-wiggle grace period
    float maxSpeed;         // Current max speed (changes with state)
    float rotation;         // Current rotation in degrees
    float targetRotation;   // Target rotation for smooth interpolation

    // Physics properties 
    float velocityX;
    float velocityY;
    float gravity;          // Gravity acceleration
    float acceleration;     // For closed arms state
    float rotationSpeed;    // For open arms state rotation

    void Init() {
        printf("SquirrelComponent::Init() called\n");
        // Gameplay state init
        state = SQUIRREL_STATE_OPEN_ARMS;
        wiggleTimer = 0;
        graceTimer = 0;
        maxSpeed = SQUIRREL_OPEN_ARMS_MAX_SPEED;  // Start with open arms speed
        rotation = 0.0f;        // Start pointing straight down
        targetRotation = 0.0f;

        // Physics init
        velocityX = 0;
        velocityY = 0;
        gravity = SQUIRREL_GRAVITY;
        acceleration = 0;
        rotationSpeed = 0;
    }

    void Destroy() override {
        Init();
    }
};

#endif 