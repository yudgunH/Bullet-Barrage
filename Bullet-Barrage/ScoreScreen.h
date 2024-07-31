#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Score.h"

class ScoreScreen {
public:
    ScoreScreen(SDL_Renderer* renderer, Score* score);
    ~ScoreScreen();

    void handleEvent(SDL_Event& e, int* currentScreen);
    void render(SDL_Renderer* renderer);
    void updateScoreTexture(SDL_Renderer* renderer);
private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    Score* score;

    SDL_Texture* backButtonTexture;
    SDL_Texture* backButtonHoverTexture;
    SDL_Rect backButtonRect;
    bool backButtonHover;

    SDL_Texture* scoreTextures[10];
    SDL_Rect scoreRects[10];

    
    void loadTextures(SDL_Renderer* renderer);
};

#endif // SCORESCREEN_H
