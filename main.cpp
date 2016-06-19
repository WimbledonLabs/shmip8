#include <iostream>
#include <SDL2/SDL.h>

#include "main.h"
#include "shm_emu_io.h"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

#define DEFAULT_WINDOW_SCALE 10

#define CHIP8_RAM_SIZE 0x1000
#define CHIP8_REGISTER_COUNT 0x10

typedef struct{
    uint8 r = 0;
    uint8 g = 0;
    uint8 b = 0;
} Pixel;

typedef union{
    uint16 keys = 0;
    struct {
        unsigned int k0: 1;
        unsigned int k1: 1;
        unsigned int k2: 1;
        unsigned int k3: 1;
        unsigned int k4: 1;
        unsigned int k5: 1;
        unsigned int k6: 1;
        unsigned int k7: 1;
        unsigned int k8: 1;
        unsigned int k9: 1;
        unsigned int kA: 1;
        unsigned int kB: 1;
        unsigned int kC: 1;
        unsigned int kD: 1;
        unsigned int kE: 1;
        unsigned int kF: 1;
    } key;
} Keyboard;


//===========Global Variables==================================================

// Not generally desirable, but these are properties of the machine that
// is being emulated, and it doesn't make sense to "pass around the world"

Pixel pixels[CHIP8_SCREEN_WIDTH][CHIP8_SCREEN_HEIGHT];
uint8 ram[CHIP8_RAM_SIZE] = {0};
uint8 reg[CHIP8_REGISTER_COUNT] = {0};

uint16 sp = 0;
uint16 pc = 0;
uint16 iAddress = 0;

uint8 delayTimer = 0;
uint8 soundTimer = 0;

OpCode opCode;

uint8 waitingForInput = 0;
uint8 waitingInputReg;

Keyboard keyboard;

//===========End Global Variables==============================================

int main() {
    //init stuff
    IO::loadROM(&romPath);
    IO::initializeDisplay(CHIP8_SCREEN_WIDTH,
                          CHIP8_SCREEN_HEIGHT,
                          DEFAULT_WINDOW_SCALE, &pixels);

    IO::initializeInput(&keyboard);

    while (1) {
        // TODO update timers
        IO::updateInput();
        if (waitingForInput && keyboard.keys) {
            for (int i=0; i<0xF; i++) {
                if (keyboard.keys & 0x1 << i) {
                    reg[waitingInputReg] = i;
                    break;
                }

                // TODO panic if something goes wrong
            }
        } else {
            opCode.value = IO::fetch(pc);
            pc += 1;
            execute(opCode);
        }

        IO::updateScreen();
        // TODO wait()
    }

    return 0;
}

void opCallRCA(OpCode op) {
    // TODO 0NNN Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
    return;
}

void opClearScreen(OpCode op) {
    // 00E0 Clears the screen.
    for (int x=0; x<CHIP8_SCREEN_WIDTH; x++) {
        for (int y=0; y<CHIP8_SCREEN_HEIGHT; y++) {
            pixels[x][y].r = 0;
            pixels[x][y].g = 0;
            pixels[x][y].b = 0;
        }
    }
}

void opReturnSubroutine(OpCode op) {
    // TODO 00EE Returns from a subroutine.
}

void opJump(OpCode op) {
    // 1NNN    Jumps to address NNN.
    pc = op.nNN;
}

void opCallSubroutine(OpCode op) {
    // TODO 2NNN    Calls subroutine at NNN.
    //
}

void opSkipEq(OpCode op) {
    // 3XNN
    // Skips the next instruction if vX equals NN
    if (reg[op.vX] == op.nN) {
        pc += 1;
    }
}

void opSkipNeq(OpCode op) {
    // 4XNN
    // Skips the next instruction if vX does not equal NN
    if (reg[op.vX] != op.nN) {
        pc += 1;
    }
}

void opSkipREq(OpCode op) {
    // 5XY0
    // Skips the next instruction if vX equals vY
    if (reg[op.vX] == reg[op.vY]) {
        pc += 1;
    }
}

void opSkipRNeq(OpCode op) {
    // 9XY0
    // Skips the next instruction if vX does not equal vY
    if (reg[op.vX] != reg[op.vY]) {
        pc += 1;
    }
}

void opMovConstant(OpCode op) {

}

void opAddConstant(OpCode op) {

}

void opMov(OpCode op) {

}

void opOr(OpCode op) {

}

void opAnd(OpCode op) {

}

void opXor(OpCode op) {

}

void opAdd(OpCode op) {

}

void opSub(OpCode op) {

}

void opRShift(OpCode op) {

}

void opRSub(OpCode op) {

}

void opLShift(OpCode op) {

}

void opSetIndex(OpCode op) {

}

void opJumpOffset(OpCode op) {

}

void opRandAnd(OpCode op) {

}

void opDraw(OpCode op) {

}

void opSkipWhenKeyDown(OpCode op) {

}

void opSkipWhenKeyUp(OpCode op) {

}

void opGetDelayTimer(OpCode op) {

}

void opWaitForKeyPress(OpCode op) {
    // FX0A
    // Pause execution until a key press is received
}

void opSetDelayTimer(OpCode op) {
    // FX15
    // Set the delay timer to vX
}

void opSetSoundTimer(OpCode op) {
    // FX18
    // Set the sound timer to vX
}

void opAddToIndex(OpCode op) {
    // FX1E
    // Adds vX to I
}

void opSetIndexToCh(OpCode op) {
    // FX29
    // Sets address I to the location where the sprite for the character for the
    // value in vX is stored
}

void opConvertToBCD(OpCode op) {
    // FX33 
    // Converts register vX into BCD with the hundreds value at address I,
    // tens values at address I+1, and the ones value at I+2
    ram[iAddress] = reg[op.vX] / 100;
    ram[iAddress+1] = ( reg[op.vX] % 100 ) / 10;
    ram[iAddress+2] = ( reg[op.vX] % 10 );
}

void opPushRange(OpCode op) {
    // FX55
    // Stores v0 to vX (inclusive) in memory starting at address I
    for (int j=0; j <= op.vX; j++) {
        ram[iAddress+j] = reg[j];
    }
}

void opPopRange(OpCode op) {
    // FX65
    // Inserts values from memory starting at address I into registers v0 to vX
    for (int j=0; j <= op.vX; j++) {
        reg[j] = ram[iAddress+j];
    }
}

void misc(OpCode op) {
    // 0x0***
    switch (op.longCode) {
        case 0xE0:
            opClearScreen(op);
            break;
        case 0xEE:
            opReturnSubroutine(op);
            break;
        default:
            opCallRCA(op);
            break;
    }
}

void arithmatic(OpCode op) {
    // 0x8XY*
    switch (op.code) {
        case 0x0:
            opMov(op);
            break;
        case 0x1:
            opOr(op);
            break;
        case 0x2:
            opAnd(op);
            break;
        case 0x3:
            opXor(op);
            break;
        case 0x4:
            opAdd(op);
            break;
        case 0x5:
            opSub(op);
            break;
        case 0x6:
            opRShift(op);
            break;
        case 0x7:
            opRSub(op);
            break;
        case 0xE:
            opLShift(op);
            break;
        default:
            // TODO panic
            break;
    }
}

void keySkip(OpCode op) {
    // 0xEX**
    switch (op.longCode) {
        case 0x9E:
            opSkipWhenKeyDown(op);
            break;
        case 0xA1:
            opSkipWhenKeyUp(op);
            break;
        default:
            // TODO panic    
            break;
    }
}

void special(OpCode op) {
    // 0xFX**
    switch (op.longCode) {
        case 0x07:
            opGetDelayTimer(op);
            break;
        case 0x0A:
            opWaitForKeyPress(op);
            break;
        case 0x15:
            opSetDelayTimer(op);
            break;
        case 0x18:
            opSetSoundTimer(op);
            break;
        case 0x1E:
            opAddToIndex(op);
            break;
        case 0x29:
            opSetIndexToCh(op);
            break;
        case 0x33:
            opConvertToBCD(op);
            break;
        case 0x55:
            opPushRange(op);
            break;
        case 0x65:
            opPopRange(op);
            break;
        default:
            // TODO panic
            break;
    }
}

void (*OpCodeTable[0x10])(OpCode) = {
    misc, //0x0
    opJump, //0x1
    opCallSubroutine, //0x2
    opSkipEq, //0x3
    opSkipNeq, //0x4
    opSkipREq, //0x5
    opMovConstant, //0x6
    opAddConstant, //0x7
    arithmatic, //0x8
    opSkipRNeq, //0x9
    opSetIndex, //0xA
    opJumpOffset, //0xB
    opRandAnd, //0xC
    opDraw, //0xD
    keySkip, //0xE
    special //0xF
};

void execute(OpCode op) {
    OpCodeTable[op.type](op);
}

/*[
        "0NNN", callRCA,
        "00E0", clearScreen,
        "00EE", returnSubroutine,
        "1NNN", jump,
        "2NNN", callSubroutine,
        "3XNN", skipEq,
        "4XNN", skipNeq,
        "5XY0", skipREq,
        "6XNN", setReg,
        "7XNN", addReg,
        "8XY0", movReg,
        "8XY1", orReg,
        "8XY2", andReg, 
        "8XY3", xorReg,  
        "8XY4", addFReg,
        "8XY5", subFReg,
        "8XY6", rshFReg,
        "8XY7", rsubFReg,
        "8XYE", lshFReg,
        "9XY0", skipRNeq,
        "ANNN", setIndex,
        "BNNN", jumpOffset,
        "CXNN", randAnd,
        "DXYN", draw,
        "EX9E", skipWithKey,
        "EXA1", skipNotKey,
        "FX07", getDelayTimer,
        "FX0A", waitForKey,
        "FX15", setDelayTimer,
        "FX18", setSoundTimer,
        "FX1E", addIndex,
        "FX29", setIndexToCh,
        "FX33", convertBCD,
        "FX55", pushRange,
        "FX65", popRange ];
}*/
