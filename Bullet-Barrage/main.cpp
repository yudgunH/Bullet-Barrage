#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Player.h"
#include "Menu.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Bullet Barrage",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            std::cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void close(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* backgroundTexture, Mix_Music* menuMusic, Mix_Music* gameMusic) {
    SDL_DestroyTexture(backgroundTexture);
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!init(&window, &renderer)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    std::string imagePath = "../assets/img/mainBackGround.png";
    std::cout << "Loading image from path: " << imagePath << std::endl;

    SDL_Texture* backgroundTexture = loadTexture(imagePath, renderer);
    if (backgroundTexture == NULL) {
        std::cerr << "Failed to load background texture!" << std::endl;
        close(window, renderer, backgroundTexture, nullptr, nullptr);
        return -1;
    }

    Mix_Music* menuMusic = Mix_LoadMUS("../assets/audio/Soundtrack - Swift Shooters.mp3");
    if (menuMusic == NULL) {
        std::cerr << "Failed to load menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        close(window, renderer, backgroundTexture, nullptr, nullptr);
        return -1;
    }

    Mix_Music* gameMusic = Mix_LoadMUS("../assets/audio/Soundtrack - Bullet Barrage.mp3");
    if (gameMusic == NULL) {
        std::cerr << "Failed to load game music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        close(window, renderer, backgroundTexture, menuMusic, nullptr);
        return -1;
    }

    Mix_PlayMusic(menuMusic, -1);

    Player player(renderer, "../assets/img/character");
    Menu menu(renderer);

    bool quit = false;
    SDL_Event e;
    int currentScreen = 0; // 0: Menu, 1: Game, 2: Score, 3: Setting

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (currentScreen == 0) {
                menu.handleEvent(e, quit, currentScreen);
                if (currentScreen == 1) {
                    Mix_HaltMusic();
                    Mix_PlayMusic(gameMusic, -1);
                }
            } else if (currentScreen == 1) {
                player.handleEvent(e);
            } else if (currentScreen == 2) {
                // Handle events for score screen (not implemented)
            } else if (currentScreen == 3) {
                // Handle events for setting screen (not implemented)
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        if (currentScreen == 0) {
            menu.render(renderer);
        } else if (currentScreen == 1) {
            player.move();
            player.render(renderer);
        } else if (currentScreen == 2) {
            // Render score screen (not implemented)
        } else if (currentScreen == 3) {
            // Render setting screen (not implemented)
        }

        SDL_RenderPresent(renderer);
    }

    close(window, renderer, backgroundTexture, menuMusic, gameMusic);
    return 0;
}
