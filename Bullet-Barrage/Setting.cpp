#include "Setting.h"
#include <iostream>
#include <SDL_image.h>

Setting::Setting(SDL_Renderer* renderer)
    : dragging(false), sliderHandleHover(false), leftButtonHover(false), rightButtonHover(false),
    backButtonHover(false), currentTrack(0), trackNameTexture(nullptr), music(nullptr), volume(100) {

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    textColor = { 0, 0, 0 };
    hoverColor = { 0, 0, 0, 255 };

    loadTextures(renderer);
    initializeTrackData();
    initializeSlider();
    initializeButtons();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    changeTrack(0);
    Mix_VolumeMusic(volume);
}

Setting::~Setting() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(sliderFrameTexture);
    SDL_DestroyTexture(sliderHandleTexture);
    SDL_DestroyTexture(sliderHandleHoverTexture);
    SDL_DestroyTexture(leftButtonTexture);
    SDL_DestroyTexture(leftButtonHoverTexture);
    SDL_DestroyTexture(rightButtonTexture);
    SDL_DestroyTexture(rightButtonHoverTexture);
    SDL_DestroyTexture(trackNameTexture);
    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(backButtonHoverTexture);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void Setting::loadTextures(SDL_Renderer* renderer) {
    backgroundTexture = IMG_LoadTexture(renderer, "../assets/img/Setting.png");

    sliderFrameTexture = IMG_LoadTexture(renderer, "../assets/img/UI/WideSlider_WhiteOutline_Frame.png");
    sliderHandleTexture = IMG_LoadTexture(renderer, "../assets/img/UI/WideSlider_Scroller.png");
    sliderHandleHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/WideSlider_Blank_Scroller.png");

    leftButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/ScrollSlider_Arrow_Left.png");
    leftButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/ScrollSlider_Blank_Arrow_Left.png");
    rightButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/ScrollSlider_Arrow_Right.png");
    rightButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/ScrollSlider_Blank_Arrow_Right.png");

    backButtonTexture = IMG_LoadTexture(renderer, "../assets/img/UI/BackButton.png");
    backButtonHoverTexture = IMG_LoadTexture(renderer, "../assets/img/UI/BackButtonHover.png");

    if (!backgroundTexture || !sliderFrameTexture || !sliderHandleTexture || !sliderHandleHoverTexture ||
        !leftButtonTexture || !leftButtonHoverTexture || !rightButtonTexture || !rightButtonHoverTexture ||
        !backButtonTexture || !backButtonHoverTexture) {
        std::cerr << "Failed to load textures! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void Setting::initializeTrackData() {
    for (const auto& entry : std::filesystem::directory_iterator("../assets/audio")) {
        trackFiles.push_back(entry.path().string());
        trackNames.push_back(entry.path().filename().string());
    }
}

void Setting::initializeSlider() {
    sliderFrameRect = { 750, 410, 400, 45 };
    sliderRect = { 764, 415, 0, 35 };
    sliderHandleRect = { 750 + 400 - 20, 410, 20, 50 };
}

void Setting::initializeButtons() {
    prevButtonRect = { 700, 540, 40, 40 };
    nextButtonRect = { 1250, 540, 40, 40 };
    backButtonRect = { 50, 50, 90, 90 };
}

void Setting::handleEvent(SDL_Event& e, bool& quit, int& volume, int& currentTrack, bool& goToMenu) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        handleSliderEvent(e, volume);
        handleButtonEvent(e, x, y, currentTrack, goToMenu);

        sliderHandleHover = (x > sliderHandleRect.x && x < sliderHandleRect.x + sliderHandleRect.w &&
            y > sliderHandleRect.y && y < sliderHandleRect.y + sliderHandleRect.h);

        leftButtonHover = (x > prevButtonRect.x && x < prevButtonRect.x + prevButtonRect.w &&
            y > prevButtonRect.y && y < prevButtonRect.y + prevButtonRect.h);
        rightButtonHover = (x > nextButtonRect.x && x < nextButtonRect.x + nextButtonRect.w &&
            y > nextButtonRect.y && y < nextButtonRect.y + nextButtonRect.h);
        backButtonHover = (x > backButtonRect.x && x < backButtonRect.x + backButtonRect.w &&
            y > backButtonRect.y && y < backButtonRect.y + backButtonRect.h);

        SDL_SetCursor(SDL_CreateSystemCursor((sliderHandleHover || leftButtonHover || rightButtonHover || backButtonHover)
            ? SDL_SYSTEM_CURSOR_HAND : SDL_SYSTEM_CURSOR_ARROW));
    }
}

void Setting::handleSliderEvent(SDL_Event& e, int& volume) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEBUTTONDOWN && sliderHandleHover) {
        dragging = true;
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        dragging = false;
    }

    if (dragging) {
        sliderHandleRect.x = x - sliderHandleRect.w / 2;

        if (sliderHandleRect.x < sliderFrameRect.x) {
            sliderHandleRect.x = sliderFrameRect.x;
        }
        else if (sliderHandleRect.x > sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w) {
            sliderHandleRect.x = sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w;
        }

        volume = (sliderHandleRect.x - sliderFrameRect.x) * 100 / (sliderFrameRect.w - sliderHandleRect.w);
        Mix_VolumeMusic(volume);
        this->volume = volume;
    }

    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x;
}

void Setting::handleButtonEvent(SDL_Event& e, int x, int y, int& currentTrack, bool& goToMenu) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (leftButtonHover) {
            currentTrack = (currentTrack - 1 + trackNames.size()) % trackNames.size();
            changeTrack(currentTrack);
        }
        if (rightButtonHover) {
            currentTrack = (currentTrack + 1) % trackNames.size();
            changeTrack(currentTrack);
        }
        if (backButtonHover) {
            goToMenu = true;
        }
    }
}

void Setting::changeTrack(int trackIndex) {
    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    music = Mix_LoadMUS(trackFiles[trackIndex].c_str());
    if (!music) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    else {
        Mix_PlayMusic(music, -1);
    }

    if (trackNameTexture) {
        SDL_DestroyTexture(trackNameTexture);
    }

    std::string trackName = trackNames[trackIndex];
    const std::string prefix = "Soundtrack - ";
    if (trackName.find(prefix) == 0) {
        trackName = trackName.substr(prefix.length());
    }

    updateTrackNameTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), trackName);
}

void Setting::updateTrackNameTexture(SDL_Renderer* renderer, const std::string& trackName) {
    std::string truncatedTrackName = trackName;
    int maxWidth = nextButtonRect.x - prevButtonRect.x - prevButtonRect.w - 20;

    int textWidth, textHeight;
    TTF_SizeText(font, truncatedTrackName.c_str(), &textWidth, &textHeight);
    while (textWidth > maxWidth) {
        truncatedTrackName = truncatedTrackName.substr(0, truncatedTrackName.length() - 4) + "...";
        TTF_SizeText(font, truncatedTrackName.c_str(), &textWidth, &textHeight);
    }

    trackNameTexture = createTextTexture(renderer, truncatedTrackName, textColor);
    trackNameRect = { (prevButtonRect.x + prevButtonRect.w + nextButtonRect.x) / 2 - textWidth / 2, prevButtonRect.y, textWidth, textHeight };
}

void Setting::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x;
    SDL_RenderFillRect(renderer, &sliderRect);

    SDL_RenderCopy(renderer, sliderFrameTexture, nullptr, &sliderFrameRect);

    SDL_RenderCopy(renderer, sliderHandleHover ? sliderHandleHoverTexture : sliderHandleTexture, nullptr, &sliderHandleRect);
    SDL_RenderCopy(renderer, leftButtonHover ? leftButtonHoverTexture : leftButtonTexture, nullptr, &prevButtonRect);
    SDL_RenderCopy(renderer, rightButtonHover ? rightButtonHoverTexture : rightButtonTexture, nullptr, &nextButtonRect);
    SDL_RenderCopy(renderer, backButtonHover ? backButtonHoverTexture : backButtonTexture, nullptr, &backButtonRect);

    if (trackNameTexture) {
        SDL_RenderCopy(renderer, trackNameTexture, nullptr, &trackNameRect);
    }

    std::string volumeText = std::to_string(volume) + "%";
    SDL_Texture* volumeTexture = createTextTexture(renderer, volumeText, textColor);
    SDL_Rect volumeRect = { sliderFrameRect.x + sliderFrameRect.w + 10, sliderFrameRect.y + 5, 50, 30 };
    SDL_RenderCopy(renderer, volumeTexture, nullptr, &volumeRect);
    SDL_DestroyTexture(volumeTexture);
}

SDL_Texture* Setting::createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color) {
    if (!font) {
        std::cerr << "Font not loaded correctly!" << std::endl;
        return nullptr;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}

int Setting::getVolume() const {
    return volume;
}

void Setting::setVolume(int volume) {
    this->volume = volume;
    Mix_VolumeMusic(volume);
    sliderHandleRect.x = sliderFrameRect.x + (volume * (sliderFrameRect.w - sliderHandleRect.w)) / 100;
    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x;
}
