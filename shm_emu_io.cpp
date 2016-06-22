#include "shm_emu_io.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <SDL2/SDL.h>

namespace IO {
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    uint8 screenScale = 1;

    Keyboard *keysPressed = nullptr;
    PixelMatrix *pixels = nullptr;

    void delay( Uint32 ms) {
        SDL_Delay(ms);
    }

    uint8 random() {
        return rand();
    }

    PixelMatrix* createPixelMatrix(uint16 width, uint16 height) {
        PixelMatrix *p = new PixelMatrix;
        if (p == nullptr) {
            return nullptr;
        }

        p->width = width;
        p->height = height;

        p->data = new Pixel*[width];
        if (p->data == nullptr) {
            return nullptr;
        }

        for (int x=0; x<width; x++) {
            p->data[x] = new Pixel[height];
            if (p->data[x] == nullptr) {
                return nullptr;
            }
        }

        // Black out pixel data
        for( int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                p->data[x][y].r = 0;
                p->data[x][y].g = 0;
                p->data[x][y].b = 0;
            }
        }

        return p;
    }

    void destoryPixelMatrix(PixelMatrix *pixelData) {
        for (int x=0; x < pixelData->width; x++) {
            delete[] pixelData->data[x];
        }

        delete[] pixelData->data;
        delete pixelData;
    }

    uint8 initializeDisplay(PixelMatrix *pixelData, uint8 scale) {
        pixels = pixelData;
        screenScale = scale;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "Failed to initialize video: " << SDL_GetError() << 
                         std::endl;
     
            return 1;
        }

        win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, 
                               SDL_WINDOWPOS_UNDEFINED, pixels->width * scale,
                               pixels->height * scale, SDL_WINDOW_SHOWN);

        if (win == nullptr) {
            std::cout << "Could not create window: " << SDL_GetError() << 
                         std::endl;
            SDL_Quit();
            return 1;
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | 
                                          SDL_RENDERER_PRESENTVSYNC);

        if (ren == nullptr) {
            SDL_DestroyWindow(win);
            std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    uint8 initializeInput(Keyboard* varWhereThePressedKeysGo) {
        keysPressed = varWhereThePressedKeysGo;
        updateInput();
        return 0;
    }

    uint8 loadROM(char* romPath) {
        return 1;
    }

    void updateInput() {
        return;
    }

    void updateScreen() {
        SDL_RenderClear(ren);
        SDL_Rect rect;
        rect.w = screenScale;
        rect.h = screenScale;
        rect.x = 0;
        for (int x=0; x < pixels->width; x++) {
            rect.y = 0;
            for (int y=0; y < pixels->height; y++) {
                Pixel p = pixels->data[x][y];
                SDL_SetRenderDrawColor(ren, p.r, p.g, p.b, 0xFF);
                SDL_RenderFillRect(ren, &rect);
                rect.y += screenScale;
            }
            rect.x += screenScale;
        }

        SDL_RenderPresent(ren);
    }

    uint16 fetch(uint16 pc) {
        return 0;
    }
}
