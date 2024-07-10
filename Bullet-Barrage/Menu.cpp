#include "Menu.h"
#include <iostream>
#include <SDL_image.h>

Menu::Menu(SDL_Renderer* renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24); // Font file path
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    textColor = { 255, 255, 255 }; 
    hoverColor = { 0, 0, 0, 255 }; 

    // Load background texture for menu
    SDL_Surface* bgSurface = IMG_Load("../assets/img/Menu.png");
    if (bgSurface == NULL) {
        std::cerr << "Unable to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    // Create buttons
    std::vector<std::string> labels = { "Play", "Score", "Setting", "Exit" }; // Adjusted order of buttons
    int startY = 600; // Adjust this value to lower the start position of the buttons
    int yIncrement = 120; // Adjust this value to control the space between the buttons
    for (int i = 0; i < labels.size(); ++i) {
        Button button;
        button.rect = { 800, startY + i * yIncrement, 320, 80 }; // Adjusted y-coordinate
        button.texture = createTextTexture(renderer, labels[i], textColor);
        if (button.texture == NULL) {
            std::cerr << "Failed to create button texture for " << labels[i] << std::endl;
            exit(1);
        }
        button.label = labels[i];
        button.hover = false;
        buttons.push_back(button);
    }
}

Menu::~Menu() {
    for (auto& button : buttons) {
        SDL_DestroyTexture(button.texture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(backgroundTexture);
}

void Menu::handleEvent(SDL_Event& e, bool& quit, int& currentScreen) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < buttons.size(); ++i) {
            if (x > buttons[i].rect.x && x < buttons[i].rect.x + buttons[i].rect.w &&
                y > buttons[i].rect.y && y < buttons[i].rect.y + buttons[i].rect.h) {
                buttons[i].hover = true;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (buttons[i].label == "Play") {
                        currentScreen = 1; // Main game screen
                    }
                    else if (buttons[i].label == "Score") {
                        currentScreen = 2; // Score screen
                    }
                    else if (buttons[i].label == "Exit") {
                        quit = true;
                    }
                    else if (buttons[i].label == "Setting") {
                        currentScreen = 3; // Setting screen
                    }
                }
            }
            else {
                buttons[i].hover = false;
            }
        }
    }
}

void Menu::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

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
        return NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);

    return textTexture;
}
