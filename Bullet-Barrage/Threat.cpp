#include "Threat.h"
#include <SDL_image.h>
#include <cmath>
#include <iostream>

Threat::Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type)
    : texture(nullptr), currentFrame(0), frameCount(6), frameWidth(16), frameHeight(16),
    lastFrameTime(0), frameDelay(100), x_pos(0), y_pos(0), velX(0.0f), velY(0.0f) {

    loadTexture(renderer, path);
    setupFrames();
}

Threat::~Threat() {
    SDL_DestroyTexture(texture);
}

void Threat::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    std::string filePath = "../assets/img/threats/" + path;
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void Threat::setupFrames() {
    for (int i = 0; i < frameCount; ++i) {
        SDL_Rect frame = { i * frameWidth, 0, frameWidth, frameHeight };
        frames.push_back(frame);
    }
}

void Threat::setPosition(float x, float y) {
    x_pos = x;
    y_pos = y;
}

void Threat::setVelocity(float x, float y) {
    velX = x;
    velY = y;
}

void Threat::update() {
    x_pos += velX;
    y_pos += velY;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % frameCount;
        lastFrameTime = currentTime;
    }
}

void Threat::render(SDL_Renderer* renderer) {
    if (texture && !frames.empty()) {
        SDL_Rect destRect = { static_cast<int>(x_pos), static_cast<int>(y_pos), frameWidth, frameHeight };
        SDL_RenderCopy(renderer, texture, &frames[currentFrame], &destRect);
    }
}

float Threat::getXPos() const {
    return x_pos;
}

float Threat::getYPos() const {
    return y_pos;
}

int Threat::getWidth() const {
    return frameWidth;
}

int Threat::getHeight() const {
    return frameHeight;
}
