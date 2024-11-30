#pragma once
#include "../base_component.h"

enum CloudType {
    CLOUD_WHITE,
    CLOUD_BLACK
};

struct CloudComponent : Component {

#define CLOUD_BOUNCE_FORCE 300.0f

    CloudType type;
    bool isBouncy;     // Only used for black clouds
    float bounceForce; // Strength of the bounce for black clouds
    bool wasHit;       // only used for white clouds
    
    void Init(CloudType cloudType) {
        type = cloudType;
        isBouncy = (type == CLOUD_BLACK);
        bounceForce = (type == CLOUD_BLACK) ? CLOUD_BOUNCE_FORCE : 0.0f; // Default bounce force
    }
    
    void Destroy() override {
        type = CLOUD_WHITE;
        isBouncy = false;
        bounceForce = 0.0f;
    }
}; 