#include "camera_system.h"
#include <stdio.h>
#include <algorithm>  // For std::min and std::max

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
            camera->targetY = targetTransform->y - camera->viewportHeight/2;

            // Smooth follow
            camera->x += (camera->targetX - camera->x) * CAMERA_FOLLOW_SPEED * deltaTime;
            camera->y += (camera->targetY - camera->y) * CAMERA_FOLLOW_SPEED * deltaTime;

            // Clamp to bounds
            camera->x = std::max(camera->minX, std::min(camera->x, camera->maxX));
            camera->y = std::max(camera->minY, std::min(camera->y, camera->maxY));
        }
    }
}

void CameraSystem::Destroy() {
    printf("CameraSystem destroyed\n");
} 