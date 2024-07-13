#include "Setting.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <filesystem>
#include "main.h"

Setting::Setting(SDL_Renderer* renderer)
    : dragging(false), sliderHandleHover(false), leftButtonHover(false), rightButtonHover(false), backButtonHover(false), currentTrack(0), trackNameTexture(nullptr), music(nullptr), volume(100) {  // Thêm khởi tạo volume

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("../assets/fonts/dlxfont_.ttf", 24); // Font file path
    if (font == NULL) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Set the text color to black
    textColor = { 0, 0, 0 };
    hoverColor = { 0, 0, 0, 255 };

    // Load background texture for setting
    SDL_Surface* bgSurface = IMG_Load("../assets/img/Setting.png");
    if (bgSurface == NULL) {
        std::cerr << "Unable to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }
    else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    // Load slider frame texture
    SDL_Surface* sliderFrameSurface = IMG_Load("../assets/img/UI/WideSlider_WhiteOutline_Frame.png");
    sliderFrameTexture = SDL_CreateTextureFromSurface(renderer, sliderFrameSurface);
    SDL_FreeSurface(sliderFrameSurface);

    // Load new slider handle texture and hover texture
    SDL_Surface* sliderSurface = IMG_Load("../assets/img/UI/WideSlider_Scroller.png");
    sliderHandleTexture = SDL_CreateTextureFromSurface(renderer, sliderSurface);
    SDL_FreeSurface(sliderSurface);

    SDL_Surface* sliderHoverSurface = IMG_Load("../assets/img/UI/WideSlider_Blank_Scroller.png");
    sliderHandleHoverTexture = SDL_CreateTextureFromSurface(renderer, sliderHoverSurface);
    SDL_FreeSurface(sliderHoverSurface);

    // Load new track button textures and hover textures
    SDL_Surface* leftButtonSurface = IMG_Load("../assets/img/UI/ScrollSlider_Arrow_Left.png");
    leftButtonTexture = SDL_CreateTextureFromSurface(renderer, leftButtonSurface);
    SDL_FreeSurface(leftButtonSurface);

    SDL_Surface* leftButtonHoverSurface = IMG_Load("../assets/img/UI/ScrollSlider_Blank_Arrow_Left.png");
    leftButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, leftButtonHoverSurface);
    SDL_FreeSurface(leftButtonHoverSurface);

    SDL_Surface* rightButtonSurface = IMG_Load("../assets/img/UI/ScrollSlider_Arrow_Right.png");
    rightButtonTexture = SDL_CreateTextureFromSurface(renderer, rightButtonSurface);
    SDL_FreeSurface(rightButtonSurface);

    SDL_Surface* rightButtonHoverSurface = IMG_Load("../assets/img/UI/ScrollSlider_Blank_Arrow_Right.png");
    rightButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, rightButtonHoverSurface);
    SDL_FreeSurface(rightButtonHoverSurface);

    // Load back button texture and hover texture
    SDL_Surface* backButtonSurface = IMG_Load("../assets/img/UI/BackButton.png");
    backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
    SDL_FreeSurface(backButtonSurface);

    SDL_Surface* backButtonHoverSurface = IMG_Load("../assets/img/UI/BackButtonHover.png");
    backButtonHoverTexture = SDL_CreateTextureFromSurface(renderer, backButtonHoverSurface);
    SDL_FreeSurface(backButtonHoverSurface);

    // Initialize slider frame and slider
    sliderFrameRect = { 750, 410, 400, 45 }; // Position and size of the slider frame
    sliderRect = { 755, 415, 0, 35 }; // Initialize width to 0
    sliderHandleRect = { 750 + 400 - 20, 410, 20, 50 }; // Position and size of the slider handle (100% volume at far right)

    // Initialize track buttons with more distance
    prevButtonRect = { 700, 540, 40, 40 }; // Position and size of the previous button
    nextButtonRect = { 1250, 540, 40, 40 }; // Position and size of the next button

    // Initialize back button at the top left corner and enlarge it
    backButtonRect = { 50, 50, 190, 190 }; // Increased size of the back button

    // Initialize track names and track files
    for (const auto& entry : std::filesystem::directory_iterator("../assets/audio")) {
        trackFiles.push_back(entry.path().string());
        trackNames.push_back(entry.path().filename().string());
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    // Load the first track
    changeTrack(0);

    // Set volume to 100%
    Mix_VolumeMusic(100);
    sliderHandleRect.x = sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w;
    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x;
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

void Setting::handleEvent(SDL_Event& e, bool& quit, int& volume, int& currentTrack, bool& goToMenu) {
    if (e.type == SDL_QUIT) {
        quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        handleSliderEvent(e, volume);
        handleButtonEvent(e, x, y, currentTrack, goToMenu);

        // Check if mouse is over slider handle
        sliderHandleHover = (x > sliderHandleRect.x && x < sliderHandleRect.x + sliderHandleRect.w &&
            y > sliderHandleRect.y && y < sliderHandleRect.y + sliderHandleRect.h);

        // Check if mouse is over track buttons
        leftButtonHover = (x > prevButtonRect.x && x < prevButtonRect.x + prevButtonRect.w &&
            y > prevButtonRect.y && y < prevButtonRect.y + prevButtonRect.h);
        rightButtonHover = (x > nextButtonRect.x && x < nextButtonRect.x + nextButtonRect.w &&
            y > nextButtonRect.y && y < nextButtonRect.y + nextButtonRect.h);

        // Check if mouse is over back button
        backButtonHover = (x > backButtonRect.x && x < backButtonRect.x + backButtonRect.w &&
            y > backButtonRect.y && y < backButtonRect.y + backButtonRect.h);

        // Change cursor to hand if hovering over slider handle or buttons
        if (sliderHandleHover || leftButtonHover || rightButtonHover || backButtonHover) {
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
        }
        else {
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
        }
    }
}

void Setting::handleSliderEvent(SDL_Event& e, int& volume) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (sliderHandleHover) {
            dragging = true;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONUP) {
        dragging = false;
    }

    if (dragging) {
        sliderHandleRect.x = x - sliderHandleRect.w / 2;

        if (sliderHandleRect.x < sliderFrameRect.x) {
            sliderHandleRect.x = sliderFrameRect.x;
        }
        if (sliderHandleRect.x > sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w) {
            sliderHandleRect.x = sliderFrameRect.x + sliderFrameRect.w - sliderHandleRect.w;
        }

        volume = (sliderHandleRect.x - sliderFrameRect.x) * 100 / (sliderFrameRect.w - sliderHandleRect.w); // Assuming volume is in percentage
        Mix_VolumeMusic(volume);
        this->volume = volume; // Update the class volume variable
    }

    // Update slider width based on handle position
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
    if (music != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }
    music = Mix_LoadMUS(trackFiles[trackIndex].c_str());
    if (music == NULL) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    else {
        Mix_PlayMusic(music, -1);
    }

    // Update track name texture
    if (trackNameTexture != nullptr) {
        SDL_DestroyTexture(trackNameTexture);
    }

    std::string trackName = trackNames[trackIndex];

    // Remove "Soundtrack - " prefix if it exists
    const std::string prefix = "Soundtrack - ";
    if (trackName.find(prefix) == 0) {
        trackName = trackName.substr(prefix.length());
    }

    // Calculate the maximum width for the track name
    int maxWidth = nextButtonRect.x - prevButtonRect.x - prevButtonRect.w - 20; // 20 pixels for padding

    // Shorten the track name if it's too long
    int textWidth, textHeight;
    TTF_SizeText(font, trackName.c_str(), &textWidth, &textHeight);
    while (textWidth > maxWidth) {
        trackName = trackName.substr(0, trackName.length() - 4) + "...";
        TTF_SizeText(font, trackName.c_str(), &textWidth, &textHeight);
    }

    trackNameTexture = createTextTexture(SDL_GetRenderer(SDL_GetWindowFromID(1)), trackName, textColor);

    // Update track name position based on texture width
    trackNameRect = { (prevButtonRect.x + prevButtonRect.w + nextButtonRect.x) / 2 - textWidth / 2, prevButtonRect.y, textWidth, textHeight };
}

void Setting::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Render slider (filled part of the slider bar)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for slider
    sliderRect.w = sliderHandleRect.x + sliderHandleRect.w / 2 - sliderRect.x; // Adjust slider length based on handle position
    SDL_RenderFillRect(renderer, &sliderRect);

    // Render slider frame on top of slider
    SDL_RenderCopy(renderer, sliderFrameTexture, NULL, &sliderFrameRect);

    // Render slider handle with hover effect
    sliderHandleRect.y = sliderFrameRect.y; // Align slider handle Y with slider frame Y
    if (sliderHandleHover) {
        SDL_RenderCopy(renderer, sliderHandleHoverTexture, NULL, &sliderHandleRect);
    }
    else {
        SDL_RenderCopy(renderer, sliderHandleTexture, NULL, &sliderHandleRect);
    }

    // Render track buttons with hover effect
    if (leftButtonHover) {
        SDL_RenderCopy(renderer, leftButtonHoverTexture, NULL, &prevButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, leftButtonTexture, NULL, &prevButtonRect);
    }

    if (rightButtonHover) {
        SDL_RenderCopy(renderer, rightButtonHoverTexture, NULL, &nextButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, rightButtonTexture, NULL, &nextButtonRect);
    }

    // Render back button with hover effect
    if (backButtonHover) {
        SDL_RenderCopy(renderer, backButtonHoverTexture, NULL, &backButtonRect);
    }
    else {
        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
    }

    // Render current track name
    if (trackNameTexture != nullptr) {
        SDL_RenderCopy(renderer, trackNameTexture, NULL, &trackNameRect);
    }

    // Render volume level
    std::string volumeText = std::to_string(volume) + "%";
    SDL_Texture* volumeTexture = createTextTexture(renderer, volumeText, textColor);
    SDL_Rect volumeRect = { sliderFrameRect.x + sliderFrameRect.w + 10, sliderFrameRect.y + 5, 50, 30 }; // Position of the volume text
    SDL_RenderCopy(renderer, volumeTexture, NULL, &volumeRect);
    SDL_DestroyTexture(volumeTexture);
}

SDL_Texture* Setting::createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color) {
    if (!font) {
        std::cerr << "Font not loaded correctly!" << std::endl;
        return NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
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
