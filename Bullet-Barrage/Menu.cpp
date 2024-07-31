#include "Menu.h"
#include "main.h"
#include <iostream>
#include <SDL_image.h>

Menu::Menu(SDL_Renderer* renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/PressStart2P-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    textColor = { 255, 255, 255 };
    hoverColor = { 0, 0, 0, 255 };

    background = new Background(renderer, "../assets/img/cities");

    std::vector<std::string> labels = { "Play", "Score", "Setting", "Exit" };
    int startY = 300;
    int yIncrement = 120;
    for (const auto& label : labels) {
        Button button;
        button.rect = { 800, startY, 320, 80 };
        button.texture = createTextTexture(renderer, label, textColor);
        if (!button.texture) {
            std::cerr << "Failed to create button texture for " << label << std::endl;
            exit(1);
        }
        button.label = label;
        button.hover = false;
        buttons.push_back(button);
        startY += yIncrement;
    }
}

Menu::~Menu() {
    for (auto& button : buttons) {
        SDL_DestroyTexture(button.texture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    delete background;
}

void Menu::handleEvent(SDL_Event& e, bool& quit, int& currentScreen) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (auto& button : buttons) {
            if (x > button.rect.x && x < button.rect.x + button.rect.w &&
                y > button.rect.y && y < button.rect.y + button.rect.h) {
                button.hover = true;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (button.label == "Play") {
                        currentScreen = GAME;
                    }
                    else if (button.label == "Score") {
                        currentScreen = SCORE;
                    }
                    else if (button.label == "Setting") {
                        currentScreen = SETTING;
                    }
                    else if (button.label == "Exit") {
                        quit = true;
                    }
                }
            }
            else {
                button.hover = false;
            }
        }
    }
}

void Menu::render(SDL_Renderer* renderer) {
    background->update();
    background->render(renderer);

    for (const auto& button : buttons) {
        if (button.hover) {
            SDL_Texture* hoverTexture = createTextTexture(renderer, button.label, hoverColor);
            SDL_RenderCopy(renderer, hoverTexture, NULL, &button.rect);
            SDL_DestroyTexture(hoverTexture);
        }
        else {
            SDL_RenderCopy(renderer, button.texture, NULL, &button.rect);
        }
    }
}

SDL_Texture* Menu::createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color) {
    if (!font) {
        std::cerr << "Font not loaded correctly!" << std::endl;
        return nullptr;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);

    return textTexture;
}
