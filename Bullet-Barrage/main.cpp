#include <SDL.h>
#include <iostream>
#include "Menu.h"
#include "Setting.h"
#include "Player.h"
#include "Background.h"

enum Screen {
    MENU,
    GAME,
    SCORE,
    SETTING
};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Bullet Barrage", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1881, 918, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    Menu menu(renderer);
    Setting setting(renderer);
    Player player(renderer, "../assets/img/character");
    Background background(renderer, "../assets/img/cities");

    bool quit = false;
    SDL_Event e;
    int currentScreen = MENU;
    int volume = 50;
    int currentTrack = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (currentScreen == MENU) {
                menu.handleEvent(e, quit, currentScreen);
            }
            else if (currentScreen == SETTING) {
                setting.handleEvent(e, quit, volume, currentTrack);
            }
            else if (currentScreen == GAME) {
                player.handleEvent(e);
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            else if (currentScreen == SCORE) {
                // Handle score screen events here
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                // Add any additional score screen event handling if needed
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (currentScreen == MENU) {
            menu.render(renderer);
        }
        else if (currentScreen == SETTING) {
            setting.render(renderer);
        }
        else if (currentScreen == GAME) {
            background.render(renderer);
            player.move();
            player.render(renderer);
        }
        else if (currentScreen == SCORE) {
            // Render score screen here
            // Add rendering logic for the score screen
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
