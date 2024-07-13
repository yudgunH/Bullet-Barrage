#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include <SDL.h>
#include "Player.h"
#include "Background.h"
#include "Threat.h"

class PlayScreen {
public:
    PlayScreen(SDL_Renderer* renderer);
    ~PlayScreen();

    void handleEvent(SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);

private:
    Player* player;
    Background* background;
    Threat* bullet;
    Threat* meteor;

    SDL_Texture* menuButtonTexture;
    SDL_Texture* menuButtonHoverTexture;
    SDL_Texture* miniMenuTexture;
    SDL_Rect menuButtonRect;
    SDL_Rect miniMenuRect;
    bool menuButtonHover;
    bool miniMenuActive;
};

#endif // PLAYSCREEN_H
