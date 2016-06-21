#ifndef SHM_EMU_IO
#define SHM_EMU_IO

#include "shmip8_common.h"

namespace IO {
    typedef struct {
        uint8 r = 0;
        uint8 g = 0;
        uint8 b = 0;
    } Pixel;

    typedef struct {
        Pixel** data;
        uint16 width;
        uint16 height;
    } PixelMatrix;

    PixelMatrix* createPixelMatrix(uint16 width, uint16 height);
    void destoryPixelMatrix(PixelMatrix *pixelData);

    uint8 initializeDisplay(PixelMatrix*, uint8 scale);

    uint8 initializeInput(Keyboard*);
    uint8 loadROM(char*);

    void updateInput();
    void updateScreen();
    uint16 fetch(uint16);
}

#endif // SHM_EMU_IO
