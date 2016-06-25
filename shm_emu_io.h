#ifndef SHM_EMU_IO
#define SHM_EMU_IO

#include "shmip8_common.h"

namespace IO {
    typedef union {
        uint32_t data = 0;
        struct {
            uint8 r;
            uint8 g;
            uint8 b;
            uint8 a;
        };
    } Pixel;

    typedef struct {
        Pixel** data;
        uint16 width;
        uint16 height;
    } PixelMatrix;

    PixelMatrix* createPixelMatrix(uint16 width, uint16 height);
    void destoryPixelMatrix(PixelMatrix *pixelData);

    void delay( uint32_t ms );
    uint8 random();

    uint8 initializeDisplay(PixelMatrix*, uint8 scale);

    uint8 initializeInput(Keyboard*);
    uint8 loadROM(char*);

    void updateInput();
    void updateScreen();
    uint16 fetch(uint16);
}

#endif // SHM_EMU_IO
