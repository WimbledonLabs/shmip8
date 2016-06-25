#ifndef SHMIP8_CORE_H
#define SHMIP8_CORE_H
#include "shmip8_common.h"
#include <fstream>

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

#define DEFAULT_WINDOW_SCALE 10

#define CHIP8_RAM_SIZE 0x1000
#define CHIP8_REGISTER_COUNT 0x10
#define CHIP8_STACK_SIZE 16

#define FONT_OFFSET 0
#define FONT_HEIGHT 5

typedef union{
    uint16 value;
    struct {
        unsigned int nNN: 12;
        unsigned int : 4;
    };

    struct {
        unsigned int nN: 8;
        unsigned int : 8;
    };
    
    struct {
        unsigned int n: 4;
        unsigned int : 12;
    };
    
    struct {
        unsigned int longCode: 8;
        unsigned int : 8;
    };

    struct {
        unsigned int code: 4;
        unsigned int vY: 4;
        unsigned int vX: 4;
        unsigned int type: 4;
    };

    struct {
        // digit3 is the least significant nibble 
        // and is last to be interpreted
        unsigned int digit3: 4;
        unsigned int digit2: 4;
        unsigned int digit1: 4;
        unsigned int digit0: 4;
    };
} OpCode;

namespace Chip8 {
    class Core {
    public:
        // NOTE: many members are publically exposed to allow for
        // modification and visualization by debuggers
        uint8 ram[CHIP8_RAM_SIZE];
        uint8 reg[CHIP8_REGISTER_COUNT];

        uint16 stack[16];
        uint8 sp;
        uint16 pc;
        uint16 I;

        uint8 dT;
        uint8 sT;

        OpCode opCode;
        HexKeyboardStatus keyboard;

        // Font data from:
        // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
        static constexpr uint8 font[] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

        static constexpr uint32 displayWidth = CHIP8_SCREEN_WIDTH;
        static constexpr uint32 displayHeight = CHIP8_SCREEN_HEIGHT;

        Core();
        ~Core();

        void loadROM(std::ifstream *rom);

        int step();
        void execute(OpCode);

        void updateInput(HexKeyboardStatus newStatus);
        void timeStep(uint32);

        uint8 screenUpdateAvailable();
        uint32* getPixelBuffer();

    private:
        uint32 *pixels;
        uint8 updateScreen;

        uint8 waitingForInput;
        uint8 waitingInputReg;

        uint16 fetch();

        // Initialization helpers
        void resetState();
        void loadFont();

        // Misc op code functionality
        void opDraw(OpCode op);
        void getKeyAfterWait();
    };
}

#endif // SHMIP8_CORE_H
