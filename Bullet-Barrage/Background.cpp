#include "Background.h"
#include <SDL_image.h>
#include <iostream>

Background::Background(SDL_Renderer* renderer, const std::string& path)
    : currentFrame(0), frameCount(81), lastFrameTime(0), frameDelay(100) {

    for (int i = 1; i <= frameCount; ++i) {
        std::string filePath = path + "/cities (" + std::to_string(i) + ").png";
        SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
        if (loadedSurface == NULL) {
            std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL) {
            std::cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else {
            frames.push_back(texture);
        }

        SDL_FreeSurface(loadedSurface);
    }

    if (frames.empty()) {
        std::cerr << "No frames loaded for background animation!" << std::endl;
    }

    // Initialize lastFrameTime to the current time to start animation immediately
    lastFrameTime = SDL_GetTicks();
}

Background::~Background() {
    for (SDL_Texture* texture : frames) {
        SDL_DestroyTexture(texture);
    }
}

void Background::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % frames.size();
        lastFrameTime = currentTime;
    }
}

void Background::render(SDL_Renderer* renderer) {
    if (!frames.empty()) {
        SDL_RenderCopy(renderer, frames[currentFrame], NULL, NULL);
    }
}

void Background::reset() {
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
}
