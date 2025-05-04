#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
SDL_Texture* renderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);
