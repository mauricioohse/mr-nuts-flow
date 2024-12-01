#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "../systems.h"
#include "../../input.h"
#include "../../resource_manager.h"

struct MusicSystem : System {
    void Init() override;
    void Update(float deltaTime, EntityManager* entities, ComponentArrays* components) override;
    void Destroy() override;
    
    void ToggleMusic();
    void PlayMusic();
    void StopMusic();
    void UpdateHelicopterSound(EntityID helicopterEntity, EntityID squirrelEntity);

private:
    bool isMusicPlaying;
    bool wasKeyPressed;  // For handling M key toggle
    SoundID backgroundMusicID;
    SoundID helicopterSoundID;
    int helicopterChannel;  // Track the channel used for helicopter sound
    bool isHelicopterPlaying;
    
    const float MAX_HELICOPTER_DISTANCE = 300.0f;  // Distance at which helicopter becomes inaudible
};

#endif 