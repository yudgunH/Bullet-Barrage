#include "PlayScreen.h"
#include "main.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib> // For srand() and rand()
#include <ctime>   // For time()

PlayScreen::PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting, Score* score)
    : renderer(renderer), menuButtonHover(false), miniMenuActive(false), homeButtonHover(false),
    returnButtonHover(false), audioButtonHover(false), audioOn(true), currentScreen(screen),
    setting(setting), previousVolume(50), pausedTime(0), elapsedTime(0),
    isPaused(false), isRunning(false), score(score) {

    srand(static_cast<unsigned>(time(0))); // Seed for random number generation

    player = new Player(renderer, "../assets/img/character");
    background = new Background(renderer, "../assets/img/cities");
    bullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);
    meteor = new Threat(renderer, "meteor.png", Threat::ThreatType::METEOR);
    kunai = new Threat(renderer, "kunai.png", Threat::ThreatType::KUNAI);
    planet = new Threat(renderer, "planet.png", Threat::ThreatType::PLANET);
    poison = new Threat(renderer, "poison.png", Threat::ThreatType::POISON);
    rocket = new Threat(renderer, "rocket.png", Threat::ThreatType::ROCKET);
    typhoon = new Threat(renderer, "typhoon.png", Threat::ThreatType::TYPHOON);
    boom = new Threat(renderer, "boom.png", Threat::ThreatType::BOOM);

    resetThreats(); // Initialize threats with random positions

    loadTextures();
    initRects();
    updateScoreTexture();
}

PlayScreen::~PlayScreen() {
    delete player;
    delete background;
    delete bullet;
    delete meteor;
    delete kunai;
    delete planet;
    delete poison;
    delete rocket;
    delete typhoon;
    delete boom;

    SDL_DestroyTexture(menuButtonTexture);
    SDL_DestroyTexture(menuButtonHoverTexture);
    SDL_DestroyTexture(miniMenuTexture);
    SDL_DestroyTexture(homeButtonTexture);
    SDL_DestroyTexture(homeButtonHoverTexture);
    SDL_DestroyTexture(returnButtonTexture);
    SDL_DestroyTexture(returnButtonHoverTexture);
    SDL_DestroyTexture(audioButtonTexture);
    SDL_DestroyTexture(audioButtonHoverTexture);
    SDL_DestroyTexture(audioButtonOffTexture);
    SDL_DestroyTexture(audioButtonOffHoverTexture);
    SDL_DestroyTexture(scoreTexture);
}

void PlayScreen::loadTextures() {
    menuButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_Menu_Grey.png");
    menuButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_Menu_Blank.png");
    miniMenuTexture = IMG_LoadTexture(renderer, "../assets/img/UI/miniMenu.png");
    homeButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Home.png");
    homeButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Home.png");
    returnButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Return.png");
    returnButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Return.png");
    audioButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Audio.png");
    audioButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Audio.png");
    audioButtonOffTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_AudioOff.png");
    audioButtonOffHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_AudioOff.png");
}

void PlayScreen::initRects() {
    SDL_Surface* miniMenuSurface = IMG_Load("../assets/img/UI/miniMenu.png");

    int originalWidth = miniMenuSurface->w;
    int originalHeight = miniMenuSurface->h;
    float aspectRatio = static_cast<float>(originalWidth) / originalHeight;

    int newWidth = 0.8 * 1881;
    int newHeight = newWidth / aspectRatio;

    if (newHeight > 0.8 * 918) {
        newHeight = 0.8 * 918;
        newWidth = newHeight * aspectRatio;
    }

    miniMenuRect = { (1881 - newWidth) / 2, (918 - newHeight) / 2, newWidth, newHeight };

    SDL_FreeSurface(miniMenuSurface);

    menuButtonRect = { 1881 - 70, 20, 50, 50 };

    int buttonWidth = 110;
    int buttonHeight = 110;
    int buttonSpacing = (miniMenuRect.w - 3 * buttonWidth) / 4;

    homeButtonRect = { miniMenuRect.x + buttonSpacing, miniMenuRect.y + miniMenuRect.h - buttonHeight - 50, buttonWidth, buttonHeight };
    returnButtonRect = { homeButtonRect.x + buttonWidth + buttonSpacing, homeButtonRect.y, buttonWidth, buttonHeight };
    audioButtonRect = { returnButtonRect.x + buttonWidth + buttonSpacing, returnButtonRect.y, buttonWidth, buttonHeight };

    TTF_Init();
    scoreRect = { menuButtonRect.x - 100, menuButtonRect.y, 90, 50 };
}

void PlayScreen::pause() {
    if (!isPaused) {
        isPaused = true;
        pausedTime = SDL_GetTicks();
    }
}

void PlayScreen::resume() {
    if (isPaused) {
        isPaused = false;
        Uint32 currentTime = SDL_GetTicks();
        startTime += (currentTime - pausedTime);
    }
}

void PlayScreen::resetScore() {
    startTime = SDL_GetTicks();
    elapsedTime = 0;
    updateScoreTexture();
}

Uint32 PlayScreen::getElapsedTime() const {
    return elapsedTime;
}

void PlayScreen::reset() {
    startTime = SDL_GetTicks();
    elapsedTime = 0;
    isPaused = false;
    isRunning = false;
    miniMenuActive = false;
    menuButtonHover = false;
    homeButtonHover = false;
    returnButtonHover = false;
    audioButtonHover = false;
    audioOn = true;
    updateScoreTexture();
    player->reset();
    background->reset();
    resetThreats(); // Reset threats to new random positions
}

void PlayScreen::resetThreats() {
    int screenWidth = 1881; // Example screen width, adjust as needed
    int screenHeight = 918; // Example screen height, adjust as needed

    bullet->setPosition(rand() % screenWidth, rand() % screenHeight);
    meteor->setPosition(rand() % screenWidth, rand() % screenHeight);
    kunai->setPosition(rand() % screenWidth, rand() % screenHeight);
    planet->setPosition(rand() % screenWidth, rand() % screenHeight);
    poison->setPosition(rand() % screenWidth, rand() % screenHeight);
    rocket->setPosition(rand() % screenWidth, rand() % screenHeight);
    typhoon->setPosition(rand() % screenWidth, rand() % screenHeight);
    boom->setPosition(rand() % screenWidth, rand() % screenHeight);
}

void PlayScreen::handleEvent(SDL_Event& e) {
    if (miniMenuActive) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            miniMenuActive = false;
            resume();
        }

        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            homeButtonHover = (x > homeButtonRect.x && x < homeButtonRect.x + homeButtonRect.w &&
                y > homeButtonRect.y && y < homeButtonRect.y + homeButtonRect.h);
            returnButtonHover = (x > returnButtonRect.x && x < returnButtonRect.x + returnButtonRect.w &&
                y > returnButtonRect.y && y < returnButtonRect.y + returnButtonRect.h);
            audioButtonHover = (x > audioButtonRect.x && x < audioButtonRect.x + audioButtonRect.w &&
                y > audioButtonRect.y && y < audioButtonRect.y + audioButtonRect.h);

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (homeButtonHover) {
                    *currentScreen = MENU;
                    miniMenuActive = false;
                    score->addScore(elapsedTime);
                    reset();
                }
                else if (returnButtonHover) {
                    miniMenuActive = false;
                    resume();
                }
                else if (audioButtonHover) {
                    if (audioOn) {
                        previousVolume = setting->getVolume();
                        setting->setVolume(0);
                    }
                    else {
                        setting->setVolume(previousVolume);
                    }
                    audioOn = !audioOn;
                }
            }
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
                pause();
            }
        }
    }

    if (!isRunning && *currentScreen == GAME) {
        startTime = SDL_GetTicks();
        isRunning = true;
    }
}

void PlayScreen::update() {
    if (!miniMenuActive && !isPaused && isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        elapsedTime = (currentTime - startTime) / 1000;
        background->update();
        player->move();

        bullet->update();
        meteor->update();
        kunai->update();
        planet->update();
        poison->update();
        rocket->update();
        typhoon->update();
        boom->update();
    }

    updateScoreTexture();
}

void PlayScreen::render(SDL_Renderer* renderer) {
    background->render(renderer);
    player->render(renderer);

    bullet->render(renderer);
    meteor->render(renderer);
    kunai->render(renderer);
    planet->render(renderer);
    poison->render(renderer);
    rocket->render(renderer);
    typhoon->render(renderer);
    boom->render(renderer);

    SDL_RenderCopy(renderer, menuButtonHover ? menuButtonHoverTexture : menuButtonTexture, nullptr, &menuButtonRect);
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    if (miniMenuActive) {
        SDL_RenderCopy(renderer, miniMenuTexture, nullptr, &miniMenuRect);

        SDL_RenderCopy(renderer, homeButtonHover ? homeButtonHoverTexture : homeButtonTexture, nullptr, &homeButtonRect);
        SDL_RenderCopy(renderer, returnButtonHover ? returnButtonHoverTexture : returnButtonTexture, nullptr, &returnButtonRect);
        SDL_RenderCopy(renderer, audioButtonHover ? (audioOn ? audioButtonHoverTexture : audioButtonOffHoverTexture) : (audioOn ? audioButtonTexture : audioButtonOffTexture), nullptr, &audioButtonRect);
    }
}

void PlayScreen::updateScoreTexture() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 displayTime = elapsedTime;
    if (!isPaused && isRunning) {
        displayTime = (currentTime - startTime) / 1000;
    }

    TTF_Font* font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 0, 0, 0, 255 };
    std::string scoreText = "Score: " + std::to_string(displayTime);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    if (!scoreTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }
}
