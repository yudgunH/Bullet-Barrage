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
        // Thêm các loại khác nếu cần
    };

    Threat(SDL_Renderer* renderer, const std::string& path, ThreatType type);
    ~Threat();

    void update();
    void render(SDL_Renderer* renderer);
    void setPosition(int x, int y);
    void setVelocity(float x, float y); // Thêm phương thức để thiết lập vận tốc

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

    Uint32 lastUpdateTime;
    ThreatType type;

    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void setupFrames();
};

#endif // THREAT_H
