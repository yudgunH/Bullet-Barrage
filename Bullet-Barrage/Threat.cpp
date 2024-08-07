#include "Threat.h"
#include <SDL_image.h>
#include <iostream>
#include <cmath>

Threat::Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type)
    : texture(nullptr), currentFrame(0), frameCount(6), frameWidth(16), frameHeight(16),
    lastFrameTime(0), frameDelay(100), x_pos(0), y_pos(0), velX(0.0f), velY(0.0f),
    lastUpdateTime(0), type(type) {

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
        frames.push_back(frame);
    }

    if (frames.empty()) {
        std::cerr << "No frames were loaded. Check the paths and files." << std::endl;
    }
}

void Threat::setVelocity(float x, float y) {
    velX = x;
    velY = y;
}

void Threat::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + frameDelay) {
        currentFrame = (currentFrame + 1) % frameCount;
        lastFrameTime = currentTime;
    }

    // Cập nhật vị trí dựa trên vận tốc hiện tại
    x_pos += velX;
    y_pos += velY;

    // Kiểm tra nếu viên đạn đã ra khỏi màn hình
    if (x_pos < 0 || x_pos > 1881 || y_pos > 918 || y_pos < 0) {
        // Logic để xóa đạn ra khỏi màn hình sẽ được xử lý trong lớp PlayScreen
    }

    lastUpdateTime = currentTime;
}

void Threat::render(SDL_Renderer* renderer) {
    if (!frames.empty()) {
        SDL_Rect destRect = { static_cast<int>(x_pos), static_cast<int>(y_pos), frameWidth, frameHeight };
        SDL_RenderCopy(renderer, texture, &frames[currentFrame], &destRect);
    }
}

void Threat::setPosition(int x, int y) {
    x_pos = static_cast<float>(x);
    y_pos = static_cast<float>(y);
}
