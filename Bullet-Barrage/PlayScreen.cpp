#include "PlayScreen.h"
#include "main.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

PlayScreen::PlayScreen(SDL_Renderer* renderer, int* screen, Setting* setting, Score* score)
    : renderer(renderer), menuButtonHover(false), miniMenuActive(false), homeButtonHover(false),
    returnButtonHover(false), audioButtonHover(false), audioOn(true), currentScreen(screen),
    setting(setting), previousVolume(50), startTime(SDL_GetTicks()), pausedTime(0),
    elapsedTime(0), isPaused(false), score(score) {

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

    // Load other button textures
    SDL_Surface* homeButtonSurface = IMG_Load("../assets/img/UI/Icon_Small_WhiteOutline_Home.png");
    homeButtonTexture = SDL_CreateTextureFromSurface(renderer, homeButtonSurface);
    SDL_FreeSurface(homeButtonSurface);

    SDL_Surface* homeButtonHoverSurface = IMG_Load("../assets/img/UI/Icon_Small_Blank_Home.png");
    homeButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, homeButtonHoverSurface);
    SDL_FreeSurface(homeButtonHoverSurface);

    SDL_Surface* returnButtonSurface = IMG_Load("../assets/img/UI/Icon_Small_WhiteOutline_Return.png");
    returnButtonTexture = SDL_CreateTextureFromSurface(renderer, returnButtonSurface);
    SDL_FreeSurface(returnButtonSurface);

    SDL_Surface* returnButtonHoverSurface = IMG_Load("../assets/img/UI/Icon_Small_Blank_Return.png");
    returnButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, returnButtonHoverSurface);
    SDL_FreeSurface(returnButtonHoverSurface);

    SDL_Surface* audioButtonSurface = IMG_Load("../assets/img/UI/Icon_Small_WhiteOutline_Audio.png");
    audioButtonTexture = SDL_CreateTextureFromSurface(renderer, audioButtonSurface);
    SDL_FreeSurface(audioButtonSurface);

    SDL_Surface* audioButtonHoverSurface = IMG_Load("../assets/img/UI/Icon_Small_Blank_Audio.png");
    audioButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, audioButtonHoverSurface);
    SDL_FreeSurface(audioButtonHoverSurface);

    SDL_Surface* audioButtonOffSurface = IMG_Load("../assets/img/UI/Icon_Small_WhiteOutline_AudioOff.png");
    audioButtonOffTexture = SDL_CreateTextureFromSurface(renderer, audioButtonOffSurface);
    SDL_FreeSurface(audioButtonOffSurface);

    SDL_Surface* audioButtonOffHoverSurface = IMG_Load("../assets/img/UI/Icon_Small_Blank_AudioOff.png");
    audioButtonOffHoverTexture = SDL_CreateTextureFromSurface(renderer, audioButtonOffHoverSurface);
    SDL_FreeSurface(audioButtonOffHoverSurface);

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

    // Initialize other buttons rects
    int buttonWidth = 110;
    int buttonHeight = 110;
    int buttonSpacing = (miniMenuRect.w - 3 * buttonWidth) / 4;

    homeButtonRect = { miniMenuRect.x + buttonSpacing, miniMenuRect.y + miniMenuRect.h - buttonHeight - 50, buttonWidth, buttonHeight };
    returnButtonRect = { homeButtonRect.x + buttonWidth + buttonSpacing, homeButtonRect.y, buttonWidth, buttonHeight };
    audioButtonRect = { returnButtonRect.x + buttonWidth + buttonSpacing, returnButtonRect.y, buttonWidth, buttonHeight };

    // Initialize score texture
    TTF_Init();
    scoreRect = { menuButtonRect.x - 100, menuButtonRect.y, 90, 50 };
    updateScoreTexture();
}

PlayScreen::~PlayScreen() {
    delete player;
    delete background;
    delete bullet;
    delete meteor;

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
        startTime += (currentTime - pausedTime);  // Điều chỉnh thời gian bắt đầu để bù cho thời gian tạm dừng
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
    miniMenuActive = false;
    menuButtonHover = false;
    homeButtonHover = false;
    returnButtonHover = false;
    audioButtonHover = false;
    audioOn = true;
    updateScoreTexture();
    player->reset();
    background->reset();
    bullet->reset();
    meteor->reset();
}

void PlayScreen::handleEvent(SDL_Event& e) {
    if (miniMenuActive) {
        // Xử lý sự kiện khi mini menu hoạt động...
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            miniMenuActive = false;
            resume();  // Resume game khi thoát mini menu
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
                    // Chuyển sang màn hình Menu
                    *currentScreen = MENU;
                    miniMenuActive = false;
                    score->addScore(elapsedTime);  // Lưu điểm số hiện tại vào Score
                    reset();  // Reset trạng thái
                }
                else if (returnButtonHover) {
                    // Tắt miniMenu và quay lại màn hình PlayScreen
                    miniMenuActive = false;
                    resume();  // Resume game
                }
                else if (audioButtonHover) {
                    // Handle audio button click
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
        // Xử lý sự kiện khi PlayScreen hoạt động bình thường...
        player->handleEvent(e);

        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            menuButtonHover = (x > menuButtonRect.x && x < menuButtonRect.x + menuButtonRect.w &&
                y > menuButtonRect.y && y < menuButtonRect.y + menuButtonRect.h);

            if (e.type == SDL_MOUSEBUTTONDOWN && menuButtonHover) {
                miniMenuActive = true;
                pause();  // Pause game khi mở mini menu
            }
        }
    }
}

void PlayScreen::update() {
    if (!miniMenuActive && !isPaused) {
        Uint32 currentTime = SDL_GetTicks();
        elapsedTime = (currentTime - startTime) / 1000;  // Cập nhật thời gian đã chơi
        background->update();
        player->move();
        bullet->update();
        meteor->update();
    }

    // Cập nhật texture của điểm số
    updateScoreTexture();
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

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);  // Hiển thị điểm số

    if (miniMenuActive) {
        SDL_RenderCopy(renderer, miniMenuTexture, NULL, &miniMenuRect);

        if (homeButtonHover) {
            SDL_RenderCopy(renderer, homeButtonHoverTexture, NULL, &homeButtonRect);
        }
        else {
            SDL_RenderCopy(renderer, homeButtonTexture, NULL, &homeButtonRect);
        }

        if (returnButtonHover) {
            SDL_RenderCopy(renderer, returnButtonHoverTexture, NULL, &returnButtonRect);
        }
        else {
            SDL_RenderCopy(renderer, returnButtonTexture, NULL, &returnButtonRect);
        }

        if (audioButtonHover) {
            if (audioOn) {
                SDL_RenderCopy(renderer, audioButtonHoverTexture, NULL, &audioButtonRect);
            }
            else {
                SDL_RenderCopy(renderer, audioButtonOffHoverTexture, NULL, &audioButtonRect);
            }
        }
        else {
            if (audioOn) {
                SDL_RenderCopy(renderer, audioButtonTexture, NULL, &audioButtonRect);
            }
            else {
                SDL_RenderCopy(renderer, audioButtonOffTexture, NULL, &audioButtonRect);
            }
        }
    }
}

void PlayScreen::updateScoreTexture() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 displayTime = elapsedTime;
    if (!isPaused) {
        displayTime = (currentTime - startTime) / 1000;
    }

    TTF_Font* font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24);
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 0, 0, 0, 255 };
    std::string scoreText = "Score: " + std::to_string(displayTime);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (scoreTexture == NULL) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}
