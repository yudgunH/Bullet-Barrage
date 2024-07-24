#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <vector>
#include <string>

class Background {
public:
    Background(SDL_Renderer* renderer, const std::string& path);
    ~Background();

    void update();
    void render(SDL_Renderer* renderer);
    void reset();

private:
    std::vector<SDL_Texture*> frames;
    int currentFrame;
    int frameCount;
    Uint32 lastFrameTime;
    Uint32 frameDelay;
};

#endif // BACKGROUND_H
