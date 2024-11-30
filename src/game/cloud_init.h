#pragma once
#include "../core/ecs/components/cloud_components.h"

struct CloudInitData {
    float x;
    float y;
    CloudType type;
};

// Define cloud layout
const CloudInitData cloudList[] = {
    {600, 600, CLOUD_WHITE},
    {200, 600, CLOUD_BLACK},
    {400, 400, CLOUD_WHITE},
    {800, 300, CLOUD_BLACK},
    // Add more clouds as needed
};


// Helper function to create clouds from initialization data
void CreateCloudsFromData(const CloudInitData* cloudList, int count); 