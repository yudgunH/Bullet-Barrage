#include "Player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer, const std::string& base_path)
    : posX(600), posY(540), velY(0), frame(0), animationSpeed(32), lastFrameTime(0), direction(RIGHT), state(IDLE), onGround(true), canDoubleJump(true), reachedPeak(false), jumpStartY(0), jumpTargetY(0), groundY(540), jumpForce(200), playerStep(5), lastStepTime(0), stepDelay(10), lastJumpTime(0), jumpDelay(20), peakDelay(200), peakTime(0), moveLeft(false), moveRight(false), lastMoveTime(0) {
    loadTextures(renderer, base_path + "/idle_L/Character_1-idle_", idleLeftTextures, 31);
    loadTextures(renderer, base_path + "/idle_R/Character_1-idle_", idleRightTextures, 31);
    loadTextures(renderer, base_path + "/Run_L/Character_1-Run_", runLeftTextures, 27);
    loadTextures(renderer, base_path + "/Run_R/Character_1-Run_", runRightTextures, 27);
    loadTextures(renderer, base_path + "/Jump_L/Character_1-jump_", jumpLeftTextures, 14);
    loadTextures(renderer, base_path + "/Jump_R/Character_1-jump_", jumpRightTextures, 14);

    float scaleFactor = 0.35;

    idleWidth = static_cast<int>(195 * scaleFactor);
    idleHeight = static_cast<int>(385 * scaleFactor);
    runWidth = static_cast<int>(327 * scaleFactor);
    runHeight = static_cast<int>(381 * scaleFactor);
    jumpWidth = static_cast<int>(361 * scaleFactor);
    jumpHeight = static_cast<int>(421 * scaleFactor);
}

Player::~Player() {
    for (auto texture : idleLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : idleRightTextures) SDL_DestroyTexture(texture);
    for (auto texture : runLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : runRightTextures) SDL_DestroyTexture(texture);
    for (auto texture : jumpLeftTextures) SDL_DestroyTexture(texture);
    for (auto texture : jumpRightTextures) SDL_DestroyTexture(texture);
}

void Player::loadTextures(SDL_Renderer* renderer, const std::string& path, std::vector<SDL_Texture*>& textures, int frameCount) {
    for (int i = 0; i < frameCount; ++i) {
        std::string filePath = path + (i < 10 ? "0" : "") + std::to_string(i) + ".png";
        SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
        if (loadedSurface == NULL) {
            std::cerr << "Unable to load image " << filePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
        }
        else {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (texture == NULL) {
                std::cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
            }
            else {
                textures.push_back(texture);
            }
            SDL_FreeSurface(loadedSurface);
        }
    }

    if (textures.empty()) {
        std::cerr << "No textures were loaded for path " << path << ". Check the paths and files." << std::endl;
    }
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            if (onGround) {
                velY = -jumpForce / 20;
                jumpStartY = posY;
                jumpTargetY = posY - jumpForce;
                onGround = false;
                state = JUMPING;
                lastJumpTime = SDL_GetTicks();
            }
            else if (!onGround && canDoubleJump) {
                velY = -jumpForce / 20;
                jumpStartY = posY;
                jumpTargetY = posY - jumpForce;
                state = JUMPING;
                canDoubleJump = false;
                lastJumpTime = SDL_GetTicks();
            }
            break;
        case SDLK_LEFT:
            moveLeft = true;
            direction = LEFT;
            if (state != JUMPING) state = RUNNING;
            break;
        case SDLK_RIGHT:
            moveRight = true;
            direction = RIGHT;
            if (state != JUMPING) state = RUNNING;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            moveLeft = false;
            if (!moveRight && state != JUMPING) state = IDLE;
            break;
        case SDLK_RIGHT:
            moveRight = false;
            if (!moveLeft && state != JUMPING) state = IDLE;
            break;
        }
    }
}

void Player::move() {
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime > lastMoveTime + stepDelay) {
        if (moveLeft) {
            posX -= playerStep;
        }
        if (moveRight) {
            posX += playerStep;
        }
        lastMoveTime = currentTime;
    }

    if (!onGround) {
        if (currentTime > lastJumpTime + jumpDelay) {
            if (!reachedPeak) {
                if (posY > jumpTargetY) {
                    posY += velY;
                }
                else {
                    posY = jumpTargetY;
                    reachedPeak = true;
                    peakTime = SDL_GetTicks();
                }
            }
            else {
                if (currentTime > peakTime + peakDelay) {
                    velY = jumpForce / 20;
                    if (posY < groundY) {
                        posY += velY;
                    }
                    else {
                        posY = groundY;
                        velY = 0;
                        onGround = true;
                        canDoubleJump = true;
                        reachedPeak = false;
                        state = IDLE;
                    }
                }
            }
            lastJumpTime = currentTime;
        }
    }

    if (posX < 0) posX = 0;
    if (posX + runWidth > 1881) posX = 1881 - runWidth;
    if (posY < 0) posY = 0;
    if (posY + jumpHeight > 918) {
        posY = 918 - jumpHeight;
        onGround = true;
        canDoubleJump = true;
        velY = 0;
        state = IDLE;
    }
}

void Player::render(SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastFrameTime + animationSpeed) {
        frame = (frame + 1) % 31;
        lastFrameTime = currentTime;
    }

    std::vector<SDL_Texture*>* currentTextures = nullptr;
    int renderWidth = 0;
    int renderHeight = 0;

    if (state == IDLE) {
        currentTextures = (direction == LEFT) ? &idleLeftTextures : &idleRightTextures;
        renderWidth = idleWidth;
        renderHeight = idleHeight;
    }
    else if (state == RUNNING) {
        currentTextures = (direction == LEFT) ? &runLeftTextures : &runRightTextures;
        renderWidth = runWidth;
        renderHeight = runHeight;
    }
    else if (state == JUMPING) {
        currentTextures = (direction == LEFT) ? &jumpLeftTextures : &jumpRightTextures;
        renderWidth = jumpWidth;
        renderHeight = jumpHeight;
    }

    if (currentTextures && !currentTextures->empty()) {
        SDL_Rect renderQuad = { posX, posY, renderWidth, renderHeight };
        SDL_RenderCopy(renderer, (*currentTextures)[frame % currentTextures->size()], NULL, &renderQuad);
    }
}

void Player::reset() {
    posX = 600;
    posY = 540;
    velY = 0;
    frame = 0;
    direction = RIGHT;
    state = IDLE;
    onGround = true;
    canDoubleJump = true;
    reachedPeak = false;
    jumpStartY = 0;
    jumpTargetY = 0;
    lastFrameTime = SDL_GetTicks();
}
