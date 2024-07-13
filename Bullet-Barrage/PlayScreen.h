#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include <SDL.h>
#include "Player.h"
#include "Background.h"
#include "Threat.h"
#include "Setting.h"
#include "main.h"

class PlayScreen {
public:
    PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting);
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
    SDL_Texture* homeButtonTexture;
    SDL_Texture* homeButtonHoverTexture;
    SDL_Texture* returnButtonTexture;
    SDL_Texture* returnButtonHoverTexture;
    SDL_Texture* audioButtonTexture;
    SDL_Texture* audioButtonHoverTexture;
    SDL_Texture* audioButtonOffTexture;
    SDL_Texture* audioButtonOffHoverTexture;
    SDL_Rect menuButtonRect;
    SDL_Rect miniMenuRect;
    SDL_Rect homeButtonRect;
    SDL_Rect returnButtonRect;
    SDL_Rect audioButtonRect;
    bool menuButtonHover;
    bool miniMenuActive;
    bool homeButtonHover;
    bool returnButtonHover;
    bool audioButtonHover;
    bool audioOn;
    int previousVolume;
    Setting* setting;  // Thêm con trỏ tới Setting
    int* currentScreen;
};

#endif // PLAYSCREEN_H
