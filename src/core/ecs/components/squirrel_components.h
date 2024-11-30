#ifndef SQUIRREL_COMPONENTS_H
#define SQUIRREL_COMPONENTS_H

#include "../base_component.h"
#include <stdio.h>
 
typedef enum {
    SQUIRREL_STATE_OPEN_ARMS,
    SQUIRREL_STATE_CLOSED_ARMS,
    SQUIRREL_STATE_WIGGLING
} SquirrelState;

struct SquirrelComponent : Component {
    // Gameplay state
    SquirrelState state;
    float wiggleTimer;      // For wiggle state duration
    float graceTimer;       // For post-wiggle grace period
    float maxSpeed;         // Current max speed (changes with state)

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
        maxSpeed = 300;

        // Physics init
        velocityX = 0;
        velocityY = 0;
        gravity = 100.0f;
        acceleration = 0;
        rotationSpeed = 0;
    }

    void Destroy() override {
        Init();
    }
};

#endif 