#include "ScoreScreen.h"
#include "main.h"
#include <SDL_image.h>
#include <iostream>

ScoreScreen::ScoreScreen(SDL_Renderer* renderer, Score* score)
    : score(score) {
    SDL_Surface* bgSurface = IMG_Load("../assets/img/ScoreBoard.png");
    if (bgSurface == NULL) {
        std::cerr << "Unable to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    scoreRect = { 100, 100, 400, 600 };  // Tùy chỉnh vị trí và kích thước của bảng điểm
    updateScoreTexture(renderer);
}

ScoreScreen::~ScoreScreen() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
}

void ScoreScreen::handleEvent(SDL_Event& e, int* currentScreen) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        *currentScreen = MENU;
    }
}

void ScoreScreen::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
}

void ScoreScreen::updateScoreTexture(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24);
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
