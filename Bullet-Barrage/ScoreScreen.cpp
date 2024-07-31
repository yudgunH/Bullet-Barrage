#include "ScoreScreen.h"
#include "main.h"
#include <SDL_image.h>
#include <iostream>

ScoreScreen::ScoreScreen(SDL_Renderer* renderer, Score* score)
    : score(score), backButtonHover(false) {

    loadTextures(renderer);
    scoreRect = { 150, 150, 1581, 600 };
    updateScoreTexture(renderer);
}

ScoreScreen::~ScoreScreen() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(backButtonHoverTexture);
    for (int i = 0; i < 10; ++i) {
        if (scoreTextures[i]) {
            SDL_DestroyTexture(scoreTextures[i]);
        }
    }
}

void ScoreScreen::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load("../assets/img/ScoreBoard.png");
    if (!bgSurface) {
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

    backButtonRect = { 50, 50, 90, 90 };
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

void ScoreScreen::updateScoreTexture(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 48);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 0, 0, 0, 255 };
    const std::vector<int>& topScores = score->getTopScores();
    std::string scoreText;

    // Clear existing score texture if any
    for (int i = 0; i < 10; ++i) {
        if (scoreTextures[i]) {
            SDL_DestroyTexture(scoreTextures[i]);
            scoreTextures[i] = nullptr;
        }
    }

    // Set custom positions for scores
    int leftX = 450;  // New X position for left column
    int rightX = 1000; // New X position for right column
    int startY = 350; // New starting Y position
    int yIncrement = 60; // New Y increment between scores

    for (size_t i = 0; i < topScores.size(); ++i) {
        scoreText = std::to_string(i + 1) + ". " + std::to_string(topScores[i]);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        scoreTextures[i] = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (i < 5) {
            scoreRects[i] = { leftX, startY + static_cast<int>(i) * yIncrement, textSurface->w, textSurface->h };
        }
        else {
            scoreRects[i] = { rightX, startY + static_cast<int>(i - 5) * yIncrement, textSurface->w, textSurface->h };
        }
        SDL_FreeSurface(textSurface);
    }

    TTF_CloseFont(font);
}

void ScoreScreen::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Render scores
    for (int i = 0; i < 10; ++i) {
        if (scoreTextures[i]) {
            SDL_RenderCopy(renderer, scoreTextures[i], NULL, &scoreRects[i]);
        }
    }

    SDL_RenderCopy(renderer, backButtonHover ? backButtonHoverTexture : backButtonTexture, NULL, &backButtonRect);
}
