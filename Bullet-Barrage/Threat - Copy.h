#ifndef THREAT_H
#define THREAT_H

#include <SDL.h>
#include <string>
#include <vector>

class Threat {
public:
    enum ThreatType {
        BULLET,
        METEOR
    };

    Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type);
    ~Threat();

    void update();
    void render(SDL_Renderer* renderer);
    void reset();

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
    float velY;
    Uint32 lastUpdateTime;
    ThreatType type;
};

#endif // THREAT_H
