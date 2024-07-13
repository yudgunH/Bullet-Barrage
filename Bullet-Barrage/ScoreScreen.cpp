#include "ScoreScreen.h"
#include "main.h"
#include <SDL_image.h>
#include <iostream>

ScoreScreen::ScoreScreen(SDL_Renderer* renderer, Score* score)
    : score(score), backButtonHover(false) {
    SDL_Surface* bgSurface = IMG_Load("../assets/img/ScoreBoard.png");
    if (bgSurface == NULL) {
        std::cerr << "Unable to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    SDL_Surface* backButtonSurface = IMG_Load("../assets/img/UI/BackButton.png");
    backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
    SDL_FreeSurface(backButtonSurface);

    SDL_Surface* backButtonHoverSurface = IMG_Load("../assets/img/UI/BackButtonHover.png");
    backButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, backButtonHoverSurface);
    SDL_FreeSurface(backButtonHoverSurface);

    backButtonRect = { 50, 50, 90, 90 };  // Điều chỉnh vị trí và kích thước của nút BackButton

    scoreRect = { 150, 150, 1581, 600 };  // Điều chỉnh vị trí và kích thước của bảng điểm cho phù hợp với khoảng trắng
    updateScoreTexture(renderer);
}

ScoreScreen::~ScoreScreen() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(backButtonHoverTexture);
}

void ScoreScreen::handleEvent(SDL_Event& e, int* currentScreen) {
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        backButtonHover = (x > backButtonRect.x && x < backButtonRect.x + backButtonRect.w &&
            y > backButtonRect.y && y < backButtonRect.y + backButtonRect.h);

        if (e.type == SDL_MOUSEBUTTONDOWN && backButtonHover) {
            *currentScreen = MENU;
        }
    }
}

void ScoreScreen::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    if (backButtonHover) {
        SDL_RenderCopy(renderer, backButtonHoverTexture, NULL, &backButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
    }
}

void ScoreScreen::updateScoreTexture(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 48);  // Điều chỉnh cỡ chữ cho phù hợp
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 0, 0, 0, 255 };
    std::string scoreText = "Top Scores:\n";
    const std::vector<int>& topScores = score->getTopScores();
    for (int i = 0; i < topScores.size(); ++i) {
        scoreText += std::to_string(i + 1) + ". " + std::to_string(topScores[i]) + "\n";
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, scoreText.c_str(), textColor, scoreRect.w);
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
