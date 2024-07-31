#include <SDL.h>
#include <iostream>
#include "Menu.h"
#include "Setting.h"
#include "PlayScreen.h"
#include "ScoreScreen.h"
#include "main.h"
#include "SDL_mixer.h"
#include "Score.h"

const std::string scoreFile = "scores.txt";

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
    Score score;
    score.loadScores(scoreFile);
    int currentScreen = MENU;
    PlayScreen playScreen(renderer, &currentScreen, &setting, &score);
    ScoreScreen scoreScreen(renderer, &score);

    bool quit = false;
    SDL_Event e;
    int volume = 50;
    int currentTrack = 0;
    bool goToMenu = false;

    setting.changeTrack(currentTrack);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                if (currentScreen == GAME) {
                    score.saveScores(scoreFile);  
                    score.loadScores(scoreFile);  
                    scoreScreen.updateScoreTexture(renderer); 
                }
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
                    playScreen.handleEvent(e);
                    if (currentScreen != GAME) {
                        score.saveScores(scoreFile);  
                        score.loadScores(scoreFile);  
                        scoreScreen.updateScoreTexture(renderer); 
                        playScreen.reset();
                    }
                    break;
                case SCORE:
                    scoreScreen.handleEvent(e, &currentScreen);
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch (currentScreen) {
        case MENU:
            menu.render(renderer);
            break;
        case SETTING:
            setting.render(renderer);
            break;
        case GAME:
            playScreen.update();
            playScreen.render(renderer);
            break;
        case SCORE:
            scoreScreen.render(renderer);
            break;
        }

        SDL_RenderPresent(renderer);
    }

    score.saveScores(scoreFile);

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
