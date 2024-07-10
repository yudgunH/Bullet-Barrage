#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <string>
#include <vector>

class Background {
public:
    Background(SDL_Renderer* renderer, const std::string& path);
    ~Background();

    void update();
    void render(SDL_Renderer* renderer);

private:
    std::vector<SDL_Texture*> frames;
    int currentFrame;
    int frameCount;
    Uint32 lastFrameTime;
    Uint32 frameDelay;
};

#endif // BACKGROUND_H
