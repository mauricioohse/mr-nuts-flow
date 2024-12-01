#include "peanut_init.h"
#include "../core/engine.h"
#include "../core/resource_manager.h"
#include <stdlib.h>
#include <time.h>

void CreatePeanutsFromData(const PeanutInitData* peanutList, int count) {
    for (int i = 0; i < count; i++) {
        EntityID peanut = g_Engine.entityManager.CreateEntity();
        
        // Select texture based on peanut type
        Texture* texture;
        switch (peanutList[i].type) {
            case PEANUT_TYPE_SHIELD:
                texture = ResourceManager::GetTexture(TEXTURE_SHIELD_PEANUT);
                break;
            case PEANUT_TYPE_SUPER:
                texture = ResourceManager::GetTexture(TEXTURE_SUPER_PEANUT);
                break;
            default:
                texture = ResourceManager::GetTexture(TEXTURE_PEANUT);
                break;
        }
        
        ADD_TRANSFORM(peanut, peanutList[i].x, peanutList[i].y, 0.0f, 1.0f);
        ADD_SPRITE(peanut, texture);
        ADD_PEANUT(peanut, peanutList[i].type);
    }
}

void GenerateRandomPeanuts(float spawnThreshold) {
    float currentHeight = spawnThreshold;  // Start a bit below the helicopter
    
    while (currentHeight < GAME_HEIGHT - spawnThreshold) {  // Stop before bottom
        // Decide if we spawn a peanut at this height
        if ((float)rand() / RAND_MAX < PEANUT_SPAWN_CHANCE) {
            EntityID peanut = g_Engine.entityManager.CreateEntity();
            
            // Random x position within reasonable bounds
            float x = 800.0f + (float)(rand() % 800);  // Between 800 and 1600
            
            // Determine peanut type
            PeanutType type;
            float typeRoll = (float)rand() / RAND_MAX;
            
            if (typeRoll < SUPER_PEANUT_CHANCE) {
                type = PEANUT_TYPE_SUPER;
            } else if (typeRoll < SUPER_PEANUT_CHANCE + SHIELD_PEANUT_CHANCE) {
                type = PEANUT_TYPE_SHIELD;
            } else {
                type = PEANUT_TYPE_REGULAR;
            }
            
            // Select texture based on type
            Texture* texture;
            switch (type) {
                case PEANUT_TYPE_SHIELD:
                    texture = ResourceManager::GetTexture(TEXTURE_SHIELD_PEANUT);
                    break;
                case PEANUT_TYPE_SUPER:
                    texture = ResourceManager::GetTexture(TEXTURE_SUPER_PEANUT);
                    break;
                default:
                    texture = ResourceManager::GetTexture(TEXTURE_PEANUT);
                    break;
            }
            
            ADD_TRANSFORM(peanut, x, currentHeight, 0.0f, 1.0f);
            ADD_SPRITE(peanut, texture);
            ADD_PEANUT(peanut, type);
            
            printf("Generated %s peanut at (%.1f, %.1f)\n", 
                type == PEANUT_TYPE_SUPER ? "super" : 
                type == PEANUT_TYPE_SHIELD ? "shield" : "regular",
                x, currentHeight);
        }
        
        currentHeight += MIN_PEANUT_SPACING;
    }
} 