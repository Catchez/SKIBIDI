#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include "config.h"
#include "init.h"
#include "texture.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) return 1;

    TTF_Font* font = TTF_OpenFont("ariblk.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* bombTex = loadTexture("bomb.png", renderer);
    SDL_Texture* bucketTex = loadTexture("bucket.jpg", renderer);
    SDL_Texture* heartTex = loadTexture("heart.png", renderer);
    SDL_Texture* bgTex = loadTexture("wallpaper.jpg", renderer);
    SDL_Texture* gameOverTex = loadTexture("gameover.jpg", renderer);

    if (!bombTex || !bucketTex || !heartTex || !bgTex) return 1;

    SDL_Rect bucket = { SCREEN_WIDTH / 2 - BUCKET_WIDTH / 2,
                        SCREEN_HEIGHT - BUCKET_HEIGHT - 20,
                        BUCKET_WIDTH, BUCKET_HEIGHT };

    SDL_Rect bomb = { rand() % (SCREEN_WIDTH - BOMB_SIZE), 0, BOMB_SIZE, BOMB_SIZE };

    int score = 0, lives = 3;
    bool quit = false, gameOver = false, started = false;
    SDL_Event e;
    srand((unsigned int)time(nullptr));

    Mix_Chunk* popSound = Mix_LoadWAV("pop.wav");
    if (!popSound) {
    std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
    return 1;
}


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                if (!started || gameOver) {
                    started = true;
                    gameOver = false;
                    score = 0;
                    lives = 3;
                    bomb.x = rand() % (SCREEN_WIDTH - BOMB_SIZE);
                    bomb.y = 0;
                }
            }
        }

        if (started && !gameOver) {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_LEFT]) {
                bucket.x -= BUCKET_SPEED;
                if (bucket.x < 0) bucket.x = 0;
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                bucket.x += BUCKET_SPEED;
                if (bucket.x > SCREEN_WIDTH - BUCKET_WIDTH)
                    bucket.x = SCREEN_WIDTH - BUCKET_WIDTH;
            }

            bomb.y += FALL_SPEED;

            if (SDL_HasIntersection(&bucket, &bomb)) {
                Mix_PlayChannel(-1, popSound, 0);
                score++;
                bomb.x = rand() % (SCREEN_WIDTH - BOMB_SIZE);
                bomb.y = 0;
                Mix_PlayChannel(-1, popSound, 0);
            } else if (bomb.y > SCREEN_HEIGHT) {
                lives--;
                bomb.x = rand() % (SCREEN_WIDTH - BOMB_SIZE);
                bomb.y = 0;
                if (lives <= 0) gameOver = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (!started) {
            SDL_RenderCopy(renderer, bgTex, NULL, NULL);
            SDL_Color col = { 255, 255, 255, 255 };
            SDL_Texture* msg = renderText("Press ENTER to Start", font, col, renderer);
            int w, h; SDL_QueryTexture(msg, NULL, NULL, &w, &h);
            SDL_Rect r = { SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h };
            SDL_RenderCopy(renderer, msg, NULL, &r);
            SDL_DestroyTexture(msg);
        }
        else if (gameOver) {
            SDL_RenderCopy(renderer, gameOverTex, NULL, NULL);
            SDL_Color col = { 255, 0, 0, 255 };
            SDL_Texture* over = renderText("Game Over! Score: " + std::to_string(score), font, col, renderer);
            int w, h; SDL_QueryTexture(over, NULL, NULL, &w, &h);
            SDL_Rect r = { SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2, w, h };
            SDL_RenderCopy(renderer, over, NULL, &r);
            SDL_DestroyTexture(over);

            SDL_Color col2 = { 0, 0, 0, 255 };
            SDL_Texture* again = renderText("Press ENTER to Retry", font, col2, renderer);
            SDL_QueryTexture(again, NULL, NULL, &w, &h);
            SDL_Rect r2 = { SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2 + 50, w, h };
            SDL_RenderCopy(renderer, again, NULL, &r2);
            SDL_DestroyTexture(again);
        }
        else {
            SDL_RenderCopy(renderer, bucketTex, NULL, &bucket);
            SDL_RenderCopy(renderer, bombTex, NULL, &bomb);

            SDL_Color col = { 0, 0, 0, 255 };
            SDL_Texture* scoreTxt = renderText("Score: " + std::to_string(score), font, col, renderer);
            SDL_Rect rs = { 10, 10, 100, 30 };
            SDL_RenderCopy(renderer, scoreTxt, NULL, &rs);
            SDL_DestroyTexture(scoreTxt);
            for (int i = 0; i < lives; ++i) {
                SDL_Rect h = { 10 + i * 30, 50, 24, 24 };
                SDL_RenderCopy(renderer, heartTex, NULL, &h);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    Mix_FreeChunk(popSound);
    SDL_DestroyTexture(bombTex);
    SDL_DestroyTexture(bucketTex);
    SDL_DestroyTexture(heartTex);
    SDL_DestroyTexture(bgTex);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
