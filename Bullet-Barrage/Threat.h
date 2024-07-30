#ifndef THREAT_H
#define THREAT_H

#include <SDL.h>
#include <string>
#include <vector>

class Threat {
public:
    enum class ThreatType {
        BULLET,
        METEOR,
        KUNAI,
        PLANET,
        POISON,
        ROCKET,
        TYPHOON,
        BOOM
    };

    Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type);
    ~Threat();

    void update();
    void render(SDL_Renderer* renderer);
    void reset();
    void setPosition(int x, int y); // New method to set the position of the threat

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

    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void setupFrames();
};

#endif // THREAT_H
