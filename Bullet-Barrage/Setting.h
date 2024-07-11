#ifndef SETTING_H
#define SETTING_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>

class Setting {
public:
    Setting(SDL_Renderer* renderer);
    ~Setting();
    void handleEvent(SDL_Event& e, bool& quit, int& volume, int& currentTrack);
    void render(SDL_Renderer* renderer);

private:
    void handleSliderEvent(SDL_Event& e, int& volume);
    void handleButtonEvent(SDL_Event& e, int x, int y, int& currentTrack);
    void changeTrack(int trackIndex);
    SDL_Texture* createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color);

    TTF_Font* font;
    SDL_Color textColor;
    SDL_Color hoverColor;

    SDL_Texture* backgroundTexture;
    SDL_Texture* sliderFrameTexture;
    SDL_Texture* sliderHandleTexture;
    SDL_Texture* sliderHandleHoverTexture;
    SDL_Texture* leftButtonTexture;
    SDL_Texture* leftButtonHoverTexture;
    SDL_Texture* rightButtonTexture;
    SDL_Texture* rightButtonHoverTexture;
    SDL_Texture* trackNameTexture;

    SDL_Rect sliderFrameRect;
    SDL_Rect sliderRect;
    SDL_Rect sliderHandleRect;
    SDL_Rect prevButtonRect;
    SDL_Rect nextButtonRect;
    SDL_Rect trackNameRect;

    bool dragging;
    bool sliderHandleHover;
    bool leftButtonHover;
    bool rightButtonHover;

    std::vector<std::string> trackNames;
    std::vector<std::string> trackFiles;
    int currentTrack;

    Mix_Music* music;
};

#endif // SETTING_H
