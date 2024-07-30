#include "Threat.h"
#include <SDL_image.h>
#include <iostream>

Threat::Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type)
    : texture(nullptr), currentFrame(0), frameCount(6), frameWidth(16), frameHeight(16),
    lastFrameTime(0), frameDelay(100), x_pos(0), y_pos(0), velX(0.5f), velY(2.0f),
    lastUpdateTime(0), type(type) {

    if (type == ThreatType::METEOR) {
        frameWidth = 15;
        frameHeight = 31;
        y_pos = -frameHeight;
    }
    else if (type == ThreatType::KUNAI || type == ThreatType::PLANET || type == ThreatType::POISON) {
        frameWidth = 15;
        frameHeight = 16;
    }
    else if (type == ThreatType::ROCKET) {
        frameWidth = 15;
        frameHeight = 40;
        frameCount = 5;
        y_pos = -frameHeight;
    }
    else if (type == ThreatType::TYPHOON) {
        frameWidth = 16;
        frameHeight = 24;
        frameCount = 2;
    }
    else if (type == ThreatType::BOOM) {
        frameWidth = 18;
        frameHeight = 15;
        frameCount = 6;
        y_pos = -frameHeight;
    }

    loadTexture(renderer, path);
    setupFrames();
}

Threat::~Threat() {
    SDL_DestroyTexture(texture);
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

void Threat::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    std::string filePath = "../assets/img/threats/" + path;
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

    if (!loadedSurface) {
        std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!texture) {
        std::cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
    }
}

void Threat::setupFrames() {
    for (int i = 0; i < frameCount; ++i) {
        SDL_Rect frame = { i * frameWidth, 0, frameWidth, frameHeight };
        if (type == ThreatType::BOOM) {
            frame = { 0, i * frameHeight, frameWidth, frameHeight }; // frames vertical for BOOM
        }
        frames.push_back(frame);
    }

    if (frames.empty()) {
        std::cerr << "No frames were loaded. Check the paths and files." << std::endl;
    }
}

void Threat::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % frameCount;
        lastFrameTime = currentTime;
    }

    if (currentTime > lastUpdateTime + 16) {
        if (type == ThreatType::BULLET || type == ThreatType::KUNAI || type == ThreatType::PLANET || type == ThreatType::POISON || type == ThreatType::TYPHOON) {
            x_pos += velX;
            if (x_pos > 1881) x_pos = -frameWidth;
        }
        else if (type == ThreatType::METEOR || type == ThreatType::ROCKET || type == ThreatType::BOOM) {
            y_pos += velY;
            if (y_pos > 918) {
                y_pos = -frameHeight;
                x_pos = rand() % (1881 - frameWidth);
            }
        }
        lastUpdateTime = currentTime;
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

void Threat::setPosition(int x, int y) {
    x_pos = static_cast<float>(x);
    y_pos = static_cast<float>(y);
}
