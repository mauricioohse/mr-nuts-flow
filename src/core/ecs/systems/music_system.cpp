#include "music_system.h"
#include <stdio.h>
#include "../../../game/game.h"
#include "math.h"

void MusicSystem::Init() {
    isMusicPlaying = false;
    wasKeyPressed = false;
    backgroundMusicID = SOUND_BACKGROUND_MUSIC;
    helicopterSoundID = SOUND_HELICOPTER;
    helicopterChannel = -1;
    isHelicopterPlaying = false;
    
    // Start playing music immediately
    PlayMusic();
    printf("MusicSystem initialized\n");
}

void MusicSystem::Update(float deltaTime, EntityManager* entities, ComponentArrays* components) {
    // Handle music toggle with M key
    if (Input::IsKeyDown(SDL_SCANCODE_M)) {
        if (!wasKeyPressed) {
            ToggleMusic();
            wasKeyPressed = true;
        }
    } else {
        wasKeyPressed = false;
    }

    // Update helicopter sound
    UpdateHelicopterSound(g_Game.helicopterEntity, g_Game.squirrelEntity);
}

void MusicSystem::UpdateHelicopterSound(EntityID helicopterEntity, EntityID squirrelEntity) {
    TransformComponent* heliTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(helicopterEntity, COMPONENT_TRANSFORM);
    TransformComponent* squirrelTransform = 
        (TransformComponent*)g_Engine.componentArrays.GetComponentData(squirrelEntity, COMPONENT_TRANSFORM);

    if (!heliTransform || !squirrelTransform) return;

    // Calculate distance between helicopter and squirrel
    float dx = heliTransform->x - squirrelTransform->x;
    float dy = heliTransform->y - squirrelTransform->y;
    float distance = sqrtf(dx * dx + dy * dy);

    // Calculate volume based on distance (0-128)
    int volume = 0;
    if (distance < MAX_HELICOPTER_DISTANCE) {
        float volumeRatio = 1.0f - (distance / MAX_HELICOPTER_DISTANCE);
        volume = (int)(volumeRatio * 128 / 4);
    }

    // Start playing helicopter sound if not already playing
    if (volume > 0) {
        if (!isHelicopterPlaying) {
            Sound* heliSound = ResourceManager::GetSound(helicopterSoundID);
            if (heliSound) {
                helicopterChannel = Mix_PlayChannel(-1, heliSound->sdlChunk, -1);  // Loop infinitely
                isHelicopterPlaying = true;
            }
        }
        // Update volume
        Mix_Volume(helicopterChannel, volume);
    } else if (isHelicopterPlaying) {
        // Stop helicopter sound when too far
        Mix_HaltChannel(helicopterChannel);
        isHelicopterPlaying = false;
    }
}

void MusicSystem::ToggleMusic() {
    if (isMusicPlaying) {
        StopMusic();
    } else {
        PlayMusic();
    }
}

void MusicSystem::PlayMusic() {
    ResourceManager::PlayMusic(backgroundMusicID, -1);  // -1 for infinite loop
    isMusicPlaying = true;
}

void MusicSystem::StopMusic() {
    ResourceManager::StopMusic();
    isMusicPlaying = false;
}

void MusicSystem::Destroy() {
    StopMusic();
    if (isHelicopterPlaying) {
        Mix_HaltChannel(helicopterChannel);
    }
    printf("MusicSystem destroyed\n");
} 