#include <stdio.h>
#include <unistd.h>
#include "shm_emu_io.h"
#include "shmip8_common.h"

#define WIDTH 100
#define HEIGHT 200
#define SCALE 5

void paint(uint32 *data, int x, int y, uint32 c) {
    data[ x + y*WIDTH ] = c;
}

int main() {
    IO::initialize();
    IO::Screen display(WIDTH, HEIGHT, SCALE);
    uint32 *pixels = new uint32[WIDTH*HEIGHT];
    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            paint(pixels, x, y, (( ( (x-50)*(x-50) + (y-50)*(y-50) ) < 150) * ( 0x00101020 * (x%16) )));
        }
    }

    paint( pixels, 50, 10, 0x00FFFFFF );

    display.updateScreen(pixels);
    display.updateScreen(pixels);
    sleep(3);
    paint( pixels, 30, 10, 0x00FFFFFF );
    display.updateScreen(pixels);
    sleep(1);
    delete[] pixels;
    pixels = nullptr;
}
