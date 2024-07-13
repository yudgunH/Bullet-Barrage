#include "PlayScreen.h"
#include <SDL_image.h>
#include <iostream>

PlayScreen::PlayScreen(SDL_Renderer* renderer)
    : menuButtonHover(false), miniMenuActive(false) {
    player = new Player(renderer, "../assets/img/character");
    background = new Background(renderer, "../assets/img/cities");
    bullet = new Threat(renderer, "bullet.png", Threat::BULLET);
    meteor = new Threat(renderer, "meteor.png", Threat::METEOR);

    // Load menu button textures
    SDL_Surface* menuButtonSurface = IMG_Load("../assets/img/UI/Icon_Large_Menu_Grey.png");
    menuButtonTexture = SDL_CreateTextureFromSurface(renderer, menuButtonSurface);
    SDL_FreeSurface(menuButtonSurface);

    SDL_Surface* menuButtonHoverSurface = IMG_Load("../assets/img/UI/Icon_Large_Menu_Blank.png");
    menuButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, menuButtonHoverSurface);
    SDL_FreeSurface(menuButtonHoverSurface);

    SDL_Surface* miniMenuSurface = IMG_Load("../assets/img/UI/miniMenu.png");
    miniMenuTexture = SDL_CreateTextureFromSurface(renderer, miniMenuSurface);

    // Calculate miniMenu size while keeping the aspect ratio
    int originalWidth = miniMenuSurface->w;
    int originalHeight = miniMenuSurface->h;
    float aspectRatio = (float)originalWidth / originalHeight;

    int newWidth = 0.8 * 1881; // 80% of screen width
    int newHeight = newWidth / aspectRatio;

    if (newHeight > 0.8 * 918) {
        newHeight = 0.8 * 918; // 80% of screen height
        newWidth = newHeight * aspectRatio;
    }

    miniMenuRect = { (1881 - newWidth) / 2, (918 - newHeight) / 2, newWidth, newHeight };

    SDL_FreeSurface(miniMenuSurface);

    // Initialize menu button rect (top-right corner)
    menuButtonRect = { 1881 - 70, 20, 50, 50 }; // Adjust the size and position as needed
}

PlayScreen::~PlayScreen() {
    delete player;
    delete background;
    delete bullet;
    delete meteor;

    SDL_DestroyTexture(menuButtonTexture);
    SDL_DestroyTexture(menuButtonHoverTexture);
    SDL_DestroyTexture(miniMenuTexture);
}

void PlayScreen::handleEvent(SDL_Event& e) {
    if (miniMenuActive) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            miniMenuActive = false;
        }
    }
    else {
        player->handleEvent(e);

        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            menuButtonHover = (x > menuButtonRect.x && x < menuButtonRect.x + menuButtonRect.w &&
                y > menuButtonRect.y && y < menuButtonRect.y + menuButtonRect.h);

            if (e.type == SDL_MOUSEBUTTONDOWN && menuButtonHover) {
                miniMenuActive = true;
            }
        }
    }
}

void PlayScreen::update() {
    if (!miniMenuActive) {
        background->update();
        player->move();
        bullet->update();
        meteor->update();
    }
}

void PlayScreen::render(SDL_Renderer* renderer) {
    background->render(renderer);
    player->render(renderer);
    bullet->render(renderer);
    meteor->render(renderer);

    if (menuButtonHover) {
        SDL_RenderCopy(renderer, menuButtonHoverTexture, NULL, &menuButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, menuButtonTexture, NULL, &menuButtonRect);
    }

    if (miniMenuActive) {
        SDL_RenderCopy(renderer, miniMenuTexture, NULL, &miniMenuRect);
    }
}
