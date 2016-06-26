#include <iostream>  // std::cout, std::endl, std::hex
#include <iomanip>   // std::setw
#include <fstream>   // std::ifstream

#include <bitset>

#include "shmip8_core.h"

using namespace Chip8;

Core::Core() {
    pixels = new uint32[CHIP8_SCREEN_WIDTH * 
                        CHIP8_SCREEN_HEIGHT * 
                        sizeof(uint32)];
    if (pixels == nullptr) {
        std::cout << "Error: Allocation for pixel data failed" << std::endl;
        exit(1);
    }

    resetState();
}

Core::~Core() {
    delete[] pixels;
}

void Core::loadFont() {
    for (int i=0; i<sizeof(this->font); i++) {
        ram[i+FONT_OFFSET] = this->font[i];
    }
}

void Core::resetState() {
    for (int i=0; i<CHIP8_RAM_SIZE; i++) {
        ram[i] = 0;
    }

    loadFont();

    for (int i=0; i<CHIP8_REGISTER_COUNT; i++) {
        reg[i] = 0;
    }

    for (int i=0; i<CHIP8_STACK_SIZE; i++) {
        stack[i] = 0;
    }

    sp = 0;
    pc = 0x200;
    I = 0;

    dT = 0;
    sT = 0;

    opCode.value = 0;

    waitingForInput = 0;
    waitingInputReg = 0;

    updateScreen = 1;

    keyboard.keys = 0;
}

uint8 Core::screenUpdateAvailable() {
    return updateScreen;
}

void Core::updateInput(HexKeyboardStatus newStatus) {
    keyboard = newStatus;
}

void Core::timeStep(uint32 step) {
    if (dT) {
        dT--;
    }

    if (sT) {
        sT--;
        if (!sT) {
            // TODO stop sound
        } else {
            // TODO play sound if sound isn't playing
        }
    }
}

uint32* Core::getPixelBuffer() {
    updateScreen = 0;
    return pixels;
}

void Core::loadROM(std::ifstream *rom) {
    char c;
    int i = 0x200;
    while (rom->get(c) && i < sizeof(ram)) {
        ram[i++] = c;
    }

    if (i >= sizeof(ram)) {
        std::cout << "Error: ROM file too large" << std::endl;
        exit(1);
    }
}

uint16 Core::fetch() {
    return ram[pc] << 8 | ram[pc+1];
}

void Core::getKeyAfterWait() {
    for (int i=0; i<0xF; i++) {
        if (keyboard.keys & 0x1 << i) {
            // Store the pressed key in the specific register
            reg[waitingInputReg] = i;
            break;
        }
        // TODO panic if something goes wrong
    }
}

int Core::step() {
    // Handle wait for keypress opcode...
    // as I write this comment I realize that this shouldn't be here...
    if (waitingForInput && keyboard.keys) {
        // Iterate through the keys until we find the pressed key
        getKeyAfterWait();
        waitingForInput = 0;
    } else {
        opCode.value = fetch();
        pc += 2;
        if (!opCode.value) {
            // TODO panic better
            std::cout << "All Zeros in op code" << std::endl;
            return 1;
        }

        //#if DEBUG_MODE == 2
            std::cout << std::setw(3) << std::hex << pc - 2 << ": " << 
                std::hex << opCode.value << std::endl;
        //#endif
        execute(opCode);
    }

    const struct timespec ts = {.tv_sec = 0, .tv_nsec = 1000000};
    nanosleep(&ts, nullptr);
}

void Core::opDraw(OpCode op) {
    // 0xDXYN
    reg[0xF] = 0;
    for (int y = 0; y < op.n; y++) {
        for (int x = 0; x < 8; x++) {
            if ( !((0x80 >> x) & ram[I+y]) ) {
                // Pixel at XY is black, no drawing
                continue;
            } else {
                //Need to draw
                uint8 xPos = (reg[op.vX] + x) % CHIP8_SCREEN_WIDTH;
                uint8 yPos = (reg[op.vY] + y) % CHIP8_SCREEN_HEIGHT;
                if (pixels[x + y*CHIP8_SCREEN_HEIGHT] & 0x00FFFFFF) {
                    // If we are drawing over a white pixel there is a collision
                    reg[0xF] = 1;
                }

                pixels[x + y*CHIP8_SCREEN_HEIGHT] ^= 0x00FFFFFF;
            }
        }
    }
    updateScreen = 1;
}

void Core::execute(OpCode op) {
    uint8 *x = &reg[op.vX];
    uint8 *y = &reg[op.vY];
    #include "opcodeSwitch.cpp"
}
