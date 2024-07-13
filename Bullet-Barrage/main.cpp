#include <SDL.h>
#include <iostream>
#include "Menu.h"
#include "Setting.h"
#include "Player.h"
#include "Background.h"
#include "Threat.h" // Thêm dòng này để sử dụng class Threat
#include "SDL_mixer.h"

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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Bullet Barrage", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1881, 918, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Menu menu(renderer);
    Setting setting(renderer);
    Player player(renderer, "../assets/img/character");
    Background background(renderer, "../assets/img/cities"); // Ensure this path is correct

    // Tạo các đối tượng Threat
    Threat bullet(renderer, "bullet.png", Threat::BULLET);
    Threat meteor(renderer, "meteor.png", Threat::METEOR);

    bool quit = false;
    SDL_Event e;
    int currentScreen = MENU;
    int volume = 50;
    int currentTrack = 0;
    bool goToMenu = false;

    // Ensure music starts when the game starts
    setting.changeTrack(currentTrack);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                switch (currentScreen) {
                case MENU:
                    menu.handleEvent(e, quit, currentScreen);
                    break;
                case SETTING:
                    setting.handleEvent(e, quit, volume, currentTrack, goToMenu);
                    if (goToMenu) {
                        currentScreen = MENU;
                        goToMenu = false;
                    }
                    break;
                case GAME:
                    player.handleEvent(e);
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    break;
                case SCORE:
                    // Handle score screen events here
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    // Add any additional score screen event handling if needed
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch (currentScreen) {
        case MENU:
            background.update();
            background.render(renderer);
            menu.render(renderer);
            break;
        case SETTING:
            setting.render(renderer);
            break;
        case GAME:
            background.update(); // Update background animation
            background.render(renderer); // Render background
            player.move();
            player.render(renderer);

            // Cập nhật và vẽ các đối tượng Threat
            bullet.update();
            bullet.render(renderer);

            meteor.update();
            meteor.render(renderer);
            break;
        case SCORE:
            // Render score screen here
            // Add rendering logic for the score screen
            break;
        }

        SDL_RenderPresent(renderer);
    }

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
