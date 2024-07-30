#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include <SDL.h>
#include "Player.h"
#include "Background.h"
#include "Threat.h"
#include "Setting.h"
#include "Score.h"
#include "Collision.h" // Add this line

class PlayScreen {
public:
    PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting, Score* score);
    ~PlayScreen();

    void handleEvent(SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);

    void pause();
    void resume();
    void resetScore();
    Uint32 getElapsedTime() const;
    void reset();

private:
    SDL_Renderer* renderer;
    Player* player;
    Background* background;
    Threat* bullet;
    Threat* meteor;
    Threat* kunai;
    Threat* planet;
    Threat* poison;
    Threat* rocket;
    Threat* typhoon;
    Threat* boom;

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
    Setting* setting;
    int* currentScreen;

    Uint32 startTime;
    Uint32 pausedTime;
    Uint32 elapsedTime;
    bool isPaused;
    bool isRunning;

    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    Score* score;

    void updateScoreTexture();
    void loadTextures();
    void initRects();
    void resetThreats();  // New method to reset threats' positions
    void handleCollisions(); // Add this method
};

#endif // PLAYSCREEN_H
