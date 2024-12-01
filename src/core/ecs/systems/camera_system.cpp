#include "camera_system.h"
#include <stdio.h>
#include <algorithm>

void CameraSystem::Init() {
    printf("CameraSystem initialized\n");
}

void CameraSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    for (EntityID entity = 1; entity < MAX_ENTITIES; entity++) {
        if (entities->HasComponent(entity, COMPONENT_CAMERA)) {
            CameraComponent* camera = &components->cameras[entity];
            
            if (camera->targetEntity == 0) continue;
            
            // Get target's transform
            TransformComponent* targetTransform = 
                (TransformComponent*)components->GetComponentData(camera->targetEntity, COMPONENT_TRANSFORM);
            
            if (!targetTransform) continue;

            // Calculate target position (center of screen)
            camera->targetX = targetTransform->x - camera->viewportWidth/2;
            camera->targetY = targetTransform->y - camera->viewportHeight/2 + 325.0f;

            // Smooth follow
            camera->x += (camera->targetX - camera->x) * CAMERA_FOLLOW_SPEED * deltaTime;
            camera->y += (camera->targetY - camera->y) * CAMERA_FOLLOW_SPEED * deltaTime;

            // clamp at the bottom
            camera->y = std::min((float)GAME_HEIGHT- 200, camera->y);

        }
    }
}

void CameraSystem::Destroy() {
    printf("CameraSystem destroyed\n");
} 