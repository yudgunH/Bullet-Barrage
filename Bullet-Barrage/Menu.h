#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "Background.h"

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void handleEvent(SDL_Event& e, bool& quit, int& currentScreen);
    void render(SDL_Renderer* renderer);

private:
    struct Button {
        SDL_Rect rect;
        SDL_Texture* texture;
        std::string label;
        bool hover;
    };

    std::vector<Button> buttons;
    TTF_Font* font;
    SDL_Color textColor;
    SDL_Color hoverColor;

    Background* background; // Add Background member

    SDL_Texture* createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color);
};

#endif // MENU_H
