#ifndef SDL_COLLECTIONS_H
#define SDL_COLLECTIONS_H

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define TEXTURES_SIZE 100

typedef struct {
    char* key;
    SDL_Texture* value;
} TextureEntry;

typedef struct {
    TextureEntry entries [TEXTURES_SIZE];
} Textures;

void texture_add(Textures* textures, TextureEntry entry);
bool texture_exist(Textures* textures, char* key);
SDL_Texture* get(Textures* textures, char* key);

#endif
