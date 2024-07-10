#include "Threat.h"
#include <SDL_image.h>
#include <iostream>

Threat::Threat(SDL_Renderer* renderer, const std::string& path)
    : texture(nullptr), currentFrame(0), frameCount(6), frameWidth(16), frameHeight(16), lastFrameTime(0), frameDelay(100),
    x_pos(0), y_pos(100), velX(0.5f), lastUpdateTime(0) {  // Adjust velX for slower speed

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == NULL) {
        std::cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
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

    // Update position with velocity
    if (currentTime > lastUpdateTime + 16) { // Update position every 16 ms (roughly 60 FPS)
        x_pos += velX;
        lastUpdateTime = currentTime;

        if (x_pos > 1920) { // Reset position if threat goes off screen
            x_pos = -frameWidth;
        }
    }
}

void Threat::render(SDL_Renderer* renderer) {
    SDL_Rect destRect = { static_cast<int>(x_pos), static_cast<int>(y_pos), frameWidth, frameHeight };
    SDL_RenderCopy(renderer, texture, &frames[currentFrame], &destRect);
}
