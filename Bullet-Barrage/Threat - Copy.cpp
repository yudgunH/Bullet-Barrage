#include "Threat.h"
#include <SDL_image.h>
#include <iostream>

Threat::Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type)
    : texture(nullptr), currentFrame(0), frameCount(6), frameWidth(16), frameHeight(16), lastFrameTime(0), frameDelay(100),
    x_pos(0), y_pos(0), velX(0.5f), velY(2.0f), lastUpdateTime(0), type(type) {

    if (type == METEOR) {
        frameWidth = 15;
        frameHeight = 31;
        y_pos = -frameHeight;
    }

    std::string filePath = "../assets/img/threats/" + path;

    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == NULL) {
        std::cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return;
    }

    SDL_FreeSurface(loadedSurface);

    for (int i = 0; i < frameCount; ++i) {
        SDL_Rect frame;
        frame.x = i * frameWidth;
        frame.y = 0;
        frame.w = frameWidth;
        frame.h = frameHeight;
        frames.push_back(frame);
    }

    if (frames.empty()) {
        std::cerr << "No frames were loaded for path " << path << ". Check the paths and files." << std::endl;
    }
}

Threat::~Threat() {
    SDL_DestroyTexture(texture);
}

void Threat::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % frameCount;
        lastFrameTime = currentTime;
    }

    if (type == BULLET) {
        if (currentTime > lastUpdateTime + 16) {
            x_pos += velX;
            lastUpdateTime = currentTime;

            if (x_pos > 1881) {
                x_pos = -frameWidth;
            }
        }
    }
    else if (type == METEOR) {
        if (currentTime > lastUpdateTime + 16) {
            y_pos += velY;
            lastUpdateTime = currentTime;

            if (y_pos > 918) {
                y_pos = -frameHeight;
                x_pos = rand() % (1881 - frameWidth);
            }
        }
    }
}

void Threat::render(SDL_Renderer* renderer) {
    if (!frames.empty()) {
        SDL_Rect destRect = { static_cast<int>(x_pos), static_cast<int>(y_pos), frameWidth, frameHeight };
        SDL_RenderCopy(renderer, texture, &frames[currentFrame], &destRect);
    }
}

void Threat::reset() {
    x_pos = 0;
    y_pos = 0;
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
}
