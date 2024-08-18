#include "PlayScreen.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cmath>

PlayScreen::PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting, Score* score)
    : renderer(renderer), currentScreen(screen), setting(setting), score(score), lastBulletTime(0),
    isPaused(false), isRunning(false), menuButtonHover(false), miniMenuActive(false), gameOverMenuActive(false),
    homeButtonHover(false), returnButtonHover(false), audioButtonHover(false), replayButtonHover(false), audioOn(true), previousVolume(50),
    startTimeOfCurrentPattern(0), currentPattern(0), isPatternActive(false), isPatternCompleted(false) {

    player = new Player(renderer, "../assets/img/character");
    background = new Background(renderer, "../assets/img/cities");
    bullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);
    boom = new Threat(renderer, "boom.png", Threat::ThreatType::BOOM);

    resetThreats();

    loadTextures();
    initRects();
    updateScoreTexture();
}

PlayScreen::~PlayScreen() {
    delete player;
    delete background;
    delete bullet;
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
    SDL_DestroyTexture(heartFullTexture);
    SDL_DestroyTexture(heartEmptyTexture);
    SDL_DestroyTexture(replayButtonTexture);
    SDL_DestroyTexture(replayButtonHoverTexture);
    SDL_DestroyTexture(scoreNotificationTexture);
    SDL_DestroyTexture(gameplayTexture);

    for (auto& bullet : bullets) {
        delete bullet;
    }
}

void PlayScreen::loadTextures() {
    menuButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_Menu_Grey.png");
    menuButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_Menu_Blank.png");
    miniMenuTexture = IMG_LoadTexture(renderer, "../assets/img/UI/miniMenu.png");
    homeButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Home.png");
    homeButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Home.png");
    returnButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Pause.png");
    returnButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Pause.png");
    audioButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Audio.png");
    audioButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Audio.png");
    audioButtonOffTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_AudioOff.png");
    audioButtonOffHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_AudioOff.png");
    heartFullTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_HeartFull.png");
    heartEmptyTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_HeartEmpty.png");
    replayButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/PremadeButtons_Replay.png");
    replayButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/PremadeButtons_Replay_Hover.png");

    // Load font và tạo texture cho thông báo điểm số
    TTF_Font* font = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 45); // Kích thước font cho scoreMessage
    SDL_Color textColor = { 0, 0, 0, 255 }; // Màu đen
    scoreNotificationTexture = nullptr;
    scoreNotificationRect = { 0, 0, 0, 0 };
    if (font) {
        std::string scoreMessage = "Your Score:" + std::to_string(lastScore);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreMessage.c_str(), textColor);
        if (textSurface) {
            scoreNotificationTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            scoreNotificationRect = { (1881 - textSurface->w) / 2, (918 - textSurface->h) / 4 + 150, textSurface->w, textSurface->h };
            SDL_FreeSurface(textSurface);
        }

        TTF_Font* gameplayFont = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 24);
        if (gameplayFont) {
            std::string gameplayText = "Gameplay:\n\nThe player must dodge bullets being fired from all directions.\n\n"
                "The objective of the game is to survive as long as possible by moving left, right, and up\n"
                "using the movement buttons to avoid getting hit.";
            SDL_Surface* textSurfaceGameplay = TTF_RenderText_Blended_Wrapped(gameplayFont, gameplayText.c_str(), textColor, 500);
            if (textSurfaceGameplay) {
                gameplayTexture = SDL_CreateTextureFromSurface(renderer, textSurfaceGameplay);
                // Cập nhật gameplayRect chỉ nếu nó chưa được khởi tạo
                if (gameplayRect.w == 0 && gameplayRect.h == 0) {
                    gameplayRect = { (1881 - textSurfaceGameplay->w) / 2, miniMenuRect.y + 235, textSurfaceGameplay->w, textSurfaceGameplay->h };
                }
                SDL_FreeSurface(textSurfaceGameplay);
            }
            TTF_CloseFont(gameplayFont);
        }

        TTF_CloseFont(font);
    }
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

    for (int i = 0; i < 3; ++i) {
        heartRects[i] = { 20 + i * 50, 20, 40, 40 };
    }

    SDL_FreeSurface(miniMenuSurface);

    menuButtonRect = { 1881 - 70, 20, 50, 50 };

    int buttonWidth = 110;
    int buttonHeight = 110;
    int buttonSpacing = (miniMenuRect.w - 3 * buttonWidth) / 4;

    homeButtonRect = { miniMenuRect.x + buttonSpacing, miniMenuRect.y + miniMenuRect.h - buttonHeight - 50, buttonWidth, buttonHeight };
    returnButtonRect = { homeButtonRect.x + buttonWidth + buttonSpacing, homeButtonRect.y, buttonWidth, buttonHeight };
    audioButtonRect = { returnButtonRect.x + buttonWidth + buttonSpacing, returnButtonRect.y, buttonWidth, buttonHeight };
    replayButtonRect = { (1881 - buttonWidth) / 2, miniMenuRect.y + miniMenuRect.h - buttonHeight - 50, buttonWidth, buttonHeight };

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
    gameOverMenuActive = false;
    menuButtonHover = false;
    homeButtonHover = false;
    returnButtonHover = false;
    replayButtonHover = false;
    audioButtonHover = false;
    audioOn = true;
    updateScoreTexture();

    player->reset();
    background->reset();

    for (auto& bullet : bullets) {
        delete bullet;
    }
    bullets.clear();

    resetThreats();
}

void PlayScreen::resetThreats() {
    int screenWidth = 1881;
    int screenHeight = 918;

    bullet->setPosition(rand() % screenWidth, rand() % screenHeight);
}

void PlayScreen::handleEvent(SDL_Event& e) {
    if (miniMenuActive) {
        if (gameOverMenuActive) {
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                homeButtonHover = (x > homeButtonRect.x && x < homeButtonRect.x + homeButtonRect.w &&
                    y > homeButtonRect.y && y < homeButtonRect.y + homeButtonRect.h);

                replayButtonHover = (x > replayButtonRect.x && x < replayButtonRect.x + replayButtonRect.w &&
                    y > replayButtonRect.y && y < replayButtonRect.y + replayButtonRect.h);

                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (homeButtonHover) {
                        *currentScreen = MENU;
                        reset();
                    }
                    else if (replayButtonHover) {
                        reset();
                        *currentScreen = GAME;
                    }
                }
            }
        }
        else {
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

void PlayScreen::createSpreadPattern(int numBullets, float speed, float angleIncrement, float angle_, float x, float y, float speedMultiplier) {
    float angle = angle_;

    for (int i = 0; i < numBullets; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        float velX = speed * cos(angle) * speedMultiplier;
        float velY = speed * sin(angle) * speedMultiplier;

        newBullet->setPosition(x, y);
        newBullet->setVelocity(velX, velY);
        bullets.push_back(newBullet);

        angle += angleIncrement;
    }
}

void PlayScreen::createRoundPattern(int numBullets, float speed, float speedMultiplier) {
    float angleIncrement = 360.0f / numBullets; // Góc tăng giữa các viên đạn
    float angle = 0.0f;

    for (int i = 0; i < numBullets; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        float velX = speed * cos(angle * M_PI / 180.0f) * speedMultiplier;
        float velY = speed * sin(angle * M_PI / 180.0f) * speedMultiplier;

        newBullet->setPosition(940, 450);
        newBullet->setVelocity(velX, velY);
        bullets.push_back(newBullet);

        angle += angleIncrement;
    }
}

void PlayScreen::createSpiralPattern(double x, double y, float speedMultiplier) {
    for (int i = 0; i <= 1; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        float radianAngle = (angle + 180.0f * i) * M_PI / 180.0f;
        float dx = std::cos(radianAngle) * maxSpeed * speedMultiplier;
        float dy = std::sin(radianAngle) * maxSpeed * speedMultiplier;

        newBullet->setPosition(x, y);
        newBullet->setVelocity(dx, dy);
        bullets.push_back(newBullet);

        angle += 10.0f;
        if (angle >= 360.0f) {
            angle = 0.0f;
        }
    }
}

void PlayScreen::createSinglePattern(float speedMultiplier) {
    int startX;
    float speed = 1.0f;

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(0)));
        seeded = true;
    }

    bool shootFromLeft = rand() % 2 == 0;

    if (shootFromLeft) {
        startX = 0;
    }
    else {
        startX = 1800;
        speed = -speed;
    }

    for (int j = 0; j < 3; ++j) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        int startY = 700 + j * 50;

        float velX = std::min(std::abs(speed), 1.0f) * (speed > 0 ? 1 : -1) * speedMultiplier;
        float velY = 0.0f;

        newBullet->setPosition(startX, startY);
        newBullet->setVelocity(velX, velY);

        bullets.push_back(newBullet);
    }
}

void PlayScreen::startNewPattern() {
    isPatternActive = true;
    isPatternCompleted = false;
    startTimeOfCurrentPattern = SDL_GetTicks();

    int previousPattern = currentPattern;
    do {
        currentPattern = rand() % 4 + 1;
    } while (currentPattern == previousPattern);

    std::cout << "Current Pattern: " << currentPattern << std::endl;
}

void PlayScreen::update() {
    if (!miniMenuActive && !isPaused && isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        background->update();
        player->updateInvincibility();
        player->move();

        int currentScore = (currentTime - startTime) / 1000;
        lastScore = currentScore; 
        float speedMultiplier = 1.0f + (currentScore / 10) * 0.1f;

        bool allBulletsOffScreen;

        if (!isPatternActive) {
            startNewPattern();
        }
        else {
            // Logic cho các pattern
            switch (currentPattern) {
            case 1:
                if (currentTime - lastSpiralBulletTime >= 200) {
                    createSpiralPattern(940, 100, speedMultiplier);
                    lastSpiralBulletTime = currentTime;
                }
                if (currentTime - startTimeOfCurrentPattern >= 6000) {
                    isPatternCompleted = true;
                }
                break;

            case 2:
                if (!isPatternCompleted) {
                    createSinglePattern(speedMultiplier);
                    isPatternCompleted = true;
                }

                allBulletsOffScreen = true;
                for (const auto& bullet : bullets) {
                    if (bullet->getXPos() >= 0 && bullet->getXPos() <= 1881 &&
                        bullet->getYPos() >= 0 && bullet->getYPos() <= 918) {
                        allBulletsOffScreen = false;
                        break;
                    }
                }
                if (allBulletsOffScreen) {
                    isPatternActive = false;
                }
                break;

            case 3:
                if (currentTime - lastBulletTime >= 1000) {
                    createRoundPattern(9, 0.3f, speedMultiplier);
                    lastBulletTime = currentTime;
                }
                if (currentTime - startTimeOfCurrentPattern >= 6000) {
                    isPatternCompleted = true;
                }
                break;

            case 4:
                if (currentTime - lastBulletTime >= 3000) {
                    createSpreadPattern(5, 0.5f, 0.3f, 0.5, 100, 100, speedMultiplier);
                    createSpreadPattern(5, 0.5f, 0.3f, M_PI / 2, 1500, 100, speedMultiplier);
                    lastBulletTime = currentTime;
                }
                if (currentTime - startTimeOfCurrentPattern >= 5000) {
                    isPatternCompleted = true;
                }
                break;
            }

            if (isPatternCompleted && currentPattern != 2) {
                isPatternActive = false;
                bullets.clear();
            }
        }

        for (auto it = bullets.begin(); it != bullets.end();) {
            (*it)->update();

            if ((*it)->getXPos() < 0 || (*it)->getXPos() > 1881 || (*it)->getYPos() > 918 || (*it)->getYPos() < 0) {
                delete* it;
                it = bullets.erase(it);
            }
            else {
                ++it;
            }
        }

        if (player->getHealth() <= 0) {
            score->addScore(currentScore);
            score->saveScores("scores.txt");
            miniMenuActive = true;
            gameOverMenuActive = true;
            isRunning = false;
            loadTextures(); 
        }

        handleCollisions();
    }

    updateScoreTexture();
}

void PlayScreen::render(SDL_Renderer* renderer) {
    background->render(renderer);
    player->render(renderer);

    for (auto& bullet : bullets) {
        bullet->render(renderer);
    }

    int currentHealth = player->getHealth();
    for (int i = 0; i < 3; ++i) {
        if (i < currentHealth) {
            SDL_RenderCopy(renderer, heartFullTexture, nullptr, &heartRects[i]);
        }
        else {
            SDL_RenderCopy(renderer, heartEmptyTexture, nullptr, &heartRects[i]);
        }
    }

    SDL_RenderCopy(renderer, menuButtonHover ? menuButtonHoverTexture : menuButtonTexture, nullptr, &menuButtonRect);
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    if (miniMenuActive) {
        SDL_RenderCopy(renderer, miniMenuTexture, nullptr, &miniMenuRect);

        if (gameOverMenuActive) {
            SDL_RenderCopy(renderer, scoreNotificationTexture, nullptr, &scoreNotificationRect);
            SDL_RenderCopy(renderer, replayButtonHover ? replayButtonHoverTexture : replayButtonTexture, nullptr, &replayButtonRect);
            SDL_RenderCopy(renderer, homeButtonHover ? homeButtonHoverTexture : homeButtonTexture, nullptr, &homeButtonRect);
        }
        else {
            SDL_RenderCopy(renderer, homeButtonHover ? homeButtonHoverTexture : homeButtonTexture, nullptr, &homeButtonRect);
            SDL_RenderCopy(renderer, returnButtonHover ? returnButtonHoverTexture : returnButtonTexture, nullptr, &returnButtonRect);
            SDL_RenderCopy(renderer, audioButtonHover ? (audioOn ? audioButtonHoverTexture : audioButtonOffHoverTexture) : (audioOn ? audioButtonTexture : audioButtonOffTexture), nullptr, &audioButtonRect);

            SDL_RenderCopy(renderer, gameplayTexture, nullptr, &gameplayRect);
        }
    }
}

void PlayScreen::updateScoreTexture() {
    if (miniMenuActive) {
        return; 
    }

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

void PlayScreen::handleCollisions() {
    SDL_Rect playerRect = { static_cast<int>(player->getPosX()), static_cast<int>(player->getPosY()), player->getWidth(), player->getHeight() };

    for (auto& bullet : bullets) {
        SDL_Rect bulletRect = { static_cast<int>(bullet->getXPos()), static_cast<int>(bullet->getYPos()), bullet->getWidth(), bullet->getHeight() };
        if (Collision::checkCollision(playerRect, bulletRect)) {
            player->reduceHealth();
        }
    }
}
