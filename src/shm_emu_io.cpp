#include "shm_emu_io.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

namespace IO {
void initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialize video: " << SDL_GetError() << 
                     std::endl;
        exit(1);
    }
}

//InputAdapter::InputAdapter() {}
InputAdapter::~InputAdapter() {}
void InputAdapter::updateAdapter(SDL_Keycode key, uint8 state) {
    std::cout << "WRONG hkeys" << std::endl;
}

void InputAdapter::getUpdates() {
    SDL_Event test_event;

    while (SDL_PollEvent(&test_event)) {
        switch (test_event.type) {
        case SDL_KEYDOWN:
            updateAdapter(test_event.key.keysym.sym, 1);
            break;
        case SDL_KEYUP:
            updateAdapter(test_event.key.keysym.sym, 0);
            break;
        case SDL_QUIT:
            exit(0);
            break;
        default:
            //std::cout << "Unhandled event: " << test_event.type << std::endl;
            break;
        }
    }
}

Screen::Screen(uint32 width, uint32 height, float scale) {
    m_width = width;
    m_height = height;
    // Create a window to draw in
    m_win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, 
                           SDL_WINDOWPOS_UNDEFINED, width * scale,
                           height * scale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (m_win == nullptr) {
        std::cout << "Could not create window: " << SDL_GetError() << 
                     std::endl;
        SDL_Quit();
        exit(1);
    }

    // Create the rendering context associated with the window
    m_ren = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED | 
                                      SDL_RENDERER_PRESENTVSYNC);

    if (m_ren == nullptr) {
        SDL_DestroyWindow(m_win);
        std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    // Black out the screen
    SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0x00);
    SDL_RenderClear(m_ren);

    // make the scaled rendering look smoother.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); 

    // Set the size of the rendered image in pixels, which will be scaled to
    // fit the window
    SDL_RenderSetLogicalSize(m_ren, width, height);

    // Create a texture to hold the pixel data while rendering
    m_tex = SDL_CreateTexture(m_ren,
                            SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            width, height);
}

Screen::~Screen() {
    return; // Do nothing I guess?
}

void Screen::updateScreen(uint32* data) {
    SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 0x00);
    SDL_RenderClear(m_ren);

    int w = m_width;
    int h = m_height;
    /* TODO add support for proper pixel buffer copying
     * =================================================== */
    SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);

    const void *pix = data;

    SDL_UpdateTexture(m_tex, NULL, pix, w * sizeof (Uint32));
    SDL_RenderClear(m_ren);
    SDL_RenderCopy(m_ren, m_tex, NULL, NULL);
    SDL_RenderPresent(m_ren);

/*
    for (int i=0; i<0x800; i++) {
        std::cout << ((data[i] & 0xffffff) ? '#' : '-');
    }


    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            uint32 p = data[x + y*h];
            int r = (p & 0x00FF0000) >> 16;
            int g = (p & 0x0000FF00) >> 8;
            int b = p & 0x000000FF;
            SDL_SetRenderDrawColor(m_ren, r, g, b, 0xFF);
            SDL_RenderDrawPoint(m_ren, x, y);

        }
    }

    SDL_RenderPresent(m_ren); */
}

}
