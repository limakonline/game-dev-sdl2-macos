#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include "collections.h"

unsigned int hash(const char* key) {
    unsigned int h = 0;

    while (*key) {
        h = (h * 31) + *key;
        key++;
    }

    return h % TEXTURES_SIZE;
}

void texture_add(Textures* textures, TextureEntry entry) {
    unsigned int h = hash(entry.key);
    textures->entries[h].key = strdup(entry.key);
    textures->entries[h].value = entry.value;
}

bool texture_exist(Textures* textures, char* key) {
    unsigned int h = hash(key);
    return (textures->entries[h].value != NULL) ? 1 : 0;
}

SDL_Texture* get(Textures* textures, char* key) {
    unsigned int h = hash(key);
    return textures->entries[h].value;
}
