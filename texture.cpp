#include "texture.h"
#include <iostream>

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        std::cerr << "CreateTexture Error: " << SDL_GetError() << std::endl;
    }
    return tex;
}

SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (!surf) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}
