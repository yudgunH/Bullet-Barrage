#ifndef THREAT_H
#define THREAT_H

#include <SDL.h>
#include <vector>
#include <string>

class Threat {
public:
    enum class ThreatType {
        BULLET,
        BOOM,
    };

    Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type);
    ~Threat();

    void update();
    void render(SDL_Renderer* renderer);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);

    float getXPos() const;
    float getYPos() const;
    int getWidth() const;
    int getHeight() const;

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

    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void setupFrames();
};

#endif // THREAT_H
