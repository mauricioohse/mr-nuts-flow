#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

// Forward declarations
struct Texture {
    SDL_Texture* sdlTexture;
    int width;
    int height;
};

struct Font {
    TTF_Font* sdlFont;
    int size;
};

struct Sound {
    Mix_Chunk* sdlChunk;
};

// Define texture IDs
enum TextureID {
    TEXTURE_NONE = 0,
    TEXTURE_BOX,
    TEXTURE_WALL,
    TEXTURE_BOX_ANIM_SHEET,
    TEXTURE_SQUIRREL_OPEN,
    TEXTURE_SQUIRREL_OPENING,
    TEXTURE_SQUIRREL_CLOSED,
    TEXTURE_MAX
};

// Define sound IDs
enum SoundID {
    SOUND_NONE = 0,
    SOUND_HIT,
    SOUND_MAX
};

// Define font IDs
enum FontID {
    FONT_NONE = 0,
    FONT_FPS,
    FONT_MAX
};

// Resource definitions
struct TextureResource {
    const char* path;
    TextureID id;
};

struct SoundResource {
    const char* path;
    SoundID id;
};

struct FontResource {
    const char* path;
    int size;
    FontID id;
};

// Global resource definitions
static const TextureResource GAME_TEXTURES[] = {
    {"assets/box.png", TEXTURE_BOX},
    {"assets/wall.png", TEXTURE_WALL},
    {"assets/box_sheet.png", TEXTURE_BOX_ANIM_SHEET},
    {"assets/sprites/squirrel/squirrel_open.png", TEXTURE_SQUIRREL_OPEN},
    {"assets/sprites/squirrel/squirrel_opening.png", TEXTURE_SQUIRREL_OPENING},
    {"assets/sprites/squirrel/squirrel_closed.png", TEXTURE_SQUIRREL_CLOSED},
    // Add new textures here
};

static const SoundResource GAME_SOUNDS[] = {
    {"assets/sounds/hit.wav", SOUND_HIT},
    // Add new sounds here
};

static const FontResource GAME_FONTS[] = {
    {"assets/fonts/VCR_OSD_MONO_1.001.ttf", 16, FONT_FPS},
    // Add new fonts here
};

struct ResourceManager {
    static void Cleanup();
    
    // Texture management
    static Texture* LoadTexture(const char* path);
    static void UnloadTexture(Texture* texture);
    static void RenderTexture(Texture* texture, int x, int y);

    // New methods for ID-based resources
    static Texture* LoadTexture(const char* path, TextureID id);
    static Texture* GetTexture(TextureID id);
    static void UnloadTexture(TextureID id);

    // Font management
    static Font* LoadFont(const char* path, int size);
    static void UnloadFont(Font* font);
    static Texture *GetTextTexture(Font *font, const char *text, SDL_Color color);
    static int RenderTextAlignedTopRight(Font* font, const char* text, SDL_Color color, int x, int y);

    // Sound management
    static Sound* LoadSound(const char* path);
    static void UnloadSound(Sound* sound);

    // New methods for ID-based resources
    static Sound* LoadSound(const char* path, SoundID id);
    static Sound* GetSound(SoundID id);
    static void UnloadSound(SoundID id);

    static Font* LoadFont(const char* path, int size, FontID id);
    static Font* GetFont(FontID id);
    static void UnloadFont(FontID id);

    // New initialization methods
    static bool InitAllResources();
    static void UnloadAllResources();
    
private:
    // Fixed-size arrays for resources
    static Texture* textures[TEXTURE_MAX];
    static Sound* sounds[SOUND_MAX];
    static Font* fonts[FONT_MAX];

    // Helper methods for initialization
    static bool InitTextures();
    static bool InitSounds();
    static bool InitFonts();
}; 