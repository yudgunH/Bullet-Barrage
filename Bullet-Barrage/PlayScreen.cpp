#include "PlayScreen.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cmath>

PlayScreen::PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting, Score* score)
    : renderer(renderer), currentScreen(screen), setting(setting), score(score), lastBulletTime(0),
    isPaused(false), isRunning(false), menuButtonHover(false), miniMenuActive(false),
    homeButtonHover(false), returnButtonHover(false), audioButtonHover(false), audioOn(true), previousVolume(50) {

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
    returnButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Return.png");
    returnButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Return.png");
    audioButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_Audio.png");
    audioButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_Audio.png");
    audioButtonOffTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_WhiteOutline_AudioOff.png");
    audioButtonOffHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Small_Blank_AudioOff.png");
    heartFullTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_HeartFull.png");
    heartEmptyTexture = IMG_LoadTexture(renderer, "../assets/img/UI/Icon_Large_HeartEmpty.png");
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
    resetThreats();
}

void PlayScreen::resetThreats() {
    int screenWidth = 1881;
    int screenHeight = 918;

    bullet->setPosition(rand() % screenWidth, rand() % screenHeight);
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


void PlayScreen::createSpreadPattern(int numBullets, float speed, float angleIncrement, float angle_, float x, float y) {
    float angle = angle_;

    for (int i = 0; i < numBullets; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        float velX = speed * cos(angle);
        float velY = speed * sin(angle);

        newBullet->setPosition(x, y);
        newBullet->setVelocity(velX, velY);
        bullets.push_back(newBullet);

        angle += angleIncrement;
    }
}


void PlayScreen::createRoundPattern(int numBullets, float speed) {
    float angleIncrement = 360.0f / numBullets; // Góc tăng giữa các viên đạn
    float angle = 0.0f;

    for (int i = 0; i < numBullets; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        // Tính toán vận tốc theo hướng xoắn ốc
        float velX = speed * cos(angle * M_PI / 180.0f);
        float velY = speed * sin(angle * M_PI / 180.0f);

        newBullet->setPosition(940, 450); // Vị trí trung tâm
        newBullet->setVelocity(velX, velY); // Đặt vận tốc theo hướng xoắn ốc
        bullets.push_back(newBullet);

        angle += angleIncrement;
    }
}

void PlayScreen::createSpiralPattern(double x, double y) {
    for (int i = 0; i <= 1; ++i) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        // Tính toán hướng đi của viên đạn
        float radianAngle = (angle + 180.0f * i) * M_PI / 180.0f;
        float dx = std::cos(radianAngle) * maxSpeed;
        float dy = std::sin(radianAngle) * maxSpeed;

        // Thiết lập vị trí và vận tốc cho viên đạn
        newBullet->setPosition(x, y); // Vị trí bắn
        newBullet->setVelocity(dx, dy);
        bullets.push_back(newBullet);


        angle += 10.0f;
        if (angle >= 360.0f) {
            angle = 0.0f;
        }
    }
}

void PlayScreen::createSinglePattern() {
    int startX;
    float speed = 1.0f;

    // Seed cho hàm rand, chỉ cần thực hiện một lần khi chương trình khởi động
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(0)));
        seeded = true;
    }

    // Random chọn bên bắn (0 hoặc 1)
    bool shootFromLeft = rand() % 2 == 0;

    if (shootFromLeft) {
        // Bắn từ trái
        startX = 100;
    }
    else {
        // Bắn từ phải
        startX = 1700;
        speed = -speed; // Đảo chiều tốc độ để bắn từ phải sang trái
    }

    // Bắn 3 viên đạn xếp thành hàng dọc
    for (int j = 0; j < 3; ++j) {
        Threat* newBullet = new Threat(renderer, "bullet.png", Threat::ThreatType::BULLET);

        int startY = 700 + j * 50; // Vị trí Y của mỗi viên đạn cách nhau 50 pixel

        // Đặt vận tốc với giới hạn
        float velX = std::min(std::abs(speed), 1.0f) * (speed > 0 ? 1 : -1);
        float velY = 0.0f;

        newBullet->setPosition(startX, startY); // Đặt vị trí bắt đầu của đạn
        newBullet->setVelocity(velX, velY); // Đặt vận tốc của đạn

        bullets.push_back(newBullet);
    }
}


void PlayScreen::update() {
    if (!miniMenuActive && !isPaused && isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        elapsedTime = (currentTime - startTime) / 1000;
        background->update();
        player->updateInvincibility();
        player->move();

        // Kiểm tra và bắn đạn cho các hàm bắn khác
        if (currentTime - lastBulletTime >= 2000) { // Fire bullets every 2 seconds
            createSpreadPattern(5, 0.5f, 0.3f, 0.5, 100, 100);
            createSpreadPattern(5, 0.5f, 0.3f, M_PI / 2, 1500, 100);
            createRoundPattern(10, 0.1f);
            createSinglePattern();
            lastBulletTime = currentTime;
        }

        // Kiểm tra và bắn đạn cho createSpiralPattern
        if (currentTime - lastSpiralBulletTime >= 200) { // Fire bullets every 0.5 seconds
            createSpiralPattern(940, 100);
            lastSpiralBulletTime = currentTime;
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

        boom->update();
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

    boom->render(renderer);

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

void PlayScreen::handleCollisions() {
    SDL_Rect playerRect = { static_cast<int>(player->getPosX()), static_cast<int>(player->getPosY()), player->getWidth(), player->getHeight() };

    for (auto& bullet : bullets) {
        SDL_Rect bulletRect = { static_cast<int>(bullet->getXPos()), static_cast<int>(bullet->getYPos()), bullet->getWidth(), bullet->getHeight() };
        if (Collision::checkCollision(playerRect, bulletRect)) {
            player->reduceHealth();
            if (player->getHealth() <= 0) {
                player->reset();
            }
        }
    }

    SDL_Rect boomRect = { static_cast<int>(boom->getXPos()), static_cast<int>(boom->getYPos()), boom->getWidth(), boom->getHeight() };
    if (Collision::checkCollision(playerRect, boomRect)) {
        player->reduceHealth();
        if (player->getHealth() <= 0) {
            player->reset();
        }
    }
}
