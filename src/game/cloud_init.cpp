#include "cloud_init.h"
#include "../core/engine.h"
#include "../core/resource_manager.h"


void CreateCloudsFromData(const CloudInitData* cloudList, int count) {
    for (int i = 0; i < count; i++) {
        const CloudInitData& data = cloudList[i];
        
        Texture* tex = (data.type == CLOUD_WHITE) ? 
            ResourceManager::GetTexture(TEXTURE_CLOUD_1) : ResourceManager::GetTexture(TEXTURE_CLOUD_2);

        EntityID cloudEntity = g_Engine.entityManager.CreateEntity();
        ADD_TRANSFORM(cloudEntity, data.x, data.y, 0, 1);
        ADD_SPRITE(cloudEntity, tex);
        ADD_CLOUD(cloudEntity, data.type);
    }
} 