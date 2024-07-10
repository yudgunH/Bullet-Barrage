#ifndef THREAT_H
#define THREAT_H

#include <SDL.h>
#include <string>
#include <vector>

class Threat {
public:
    Threat(SDL_Renderer* renderer, const std::string& path);
    ~Threat();

    void update();
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
    std::vector<SDL_Rect> frames;
    int currentFrame;
    int frameCount;
    int frameWidth;
    int frameHeight;
    Uint32 lastFrameTime;
    Uint32 frameDelay;

    float x_pos;
    float y_pos;
    float velX;
    Uint32 lastUpdateTime;
};

#endif // THREAT_H
