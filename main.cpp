#include <iostream>  // std::cout, std::endl, std::hex
#include <iomanip>   // std::setw
#include <fstream>   // std::ifstream

#include "main.h"
#include "shm_emu_io.h"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

#define DEFAULT_WINDOW_SCALE 10

#define CHIP8_RAM_SIZE 0x1000
#define CHIP8_REGISTER_COUNT 0x10

#define FONT_OFFSET 0
#define FONT_HEIGHT 5

//===========Global Variables==================================================

// Not generally desirable, but these are properties of the machine that
// is being emulated, and it doesn't make sense to "pass around the world"

IO::PixelMatrix *pixels = nullptr;
uint8 ram[CHIP8_RAM_SIZE] = {0};
uint8 reg[CHIP8_REGISTER_COUNT] = {0};

uint16 stack[16] = {0};
uint8 sp = 0;
uint16 pc = 0;
uint16 iAddress = 0;

uint8 delayTimer = 0;
uint8 soundTimer = 0;

OpCode opCode;

uint8 waitingForInput = 0;
uint8 waitingInputReg;

uint8 updateScreen = 0;

Keyboard keyboard;

// Font data from:
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
uint8 font[] = {
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

//===========End Global Variables==============================================

void loadFont() {
    for (int i=0; i<sizeof(font); i++) {
        ram[i+FONT_OFFSET] = font[i];
    }
}

int init(char* romPath) {
    pixels = IO::createPixelMatrix(CHIP8_SCREEN_WIDTH, CHIP8_SCREEN_HEIGHT);
    std::ifstream romFile(romPath);
    if (pixels == nullptr) {
        std::cout << "Error: Allocation for pixel data failed" << std::endl;
        return 1;
    }

    if (!romFile.is_open()) {
        std::cout << "Error: Opening ROM failed" << std::endl;
        return 1;
    }

    if (IO::initializeDisplay(pixels, DEFAULT_WINDOW_SCALE) != 0) {
        std::cout << "Error: Display initialization failed" << std::endl;
        return 1;
    }

    if (IO::initializeInput(&keyboard) != 0) {
        std::cout << "Error: Input initialization failed" << std::endl;
        return 1;
    }

    uint16 i = 0x200;
    char c;
    while (romFile.get(c) && i<sizeof(ram)) {
        ram[i++] = c;
    }

    if (i >= sizeof(ram)) {
        std::cout << "Error: ROM file too large" << std::endl;
        return 1;
    }

    loadFont();

    updateScreen = 1;
    pc = 0x200;
    sp = 0;
    return 0;
}

uint16 fetch(uint16 programCounter) {
    return ram[programCounter] << 8 | ram[programCounter+1];
}

void printStatus() {
    std::cout << "Current Instruction: 0x" << std::hex << fetch(pc) << std::endl;

    std::cout << "v0: 0x" << std::hex << static_cast<int>(reg[0x0]) << std::endl;
    std::cout << "v1: 0x" << std::hex << static_cast<int>(reg[0x1]) << std::endl;
    std::cout << "v2: 0x" << std::hex << static_cast<int>(reg[0x2]) << std::endl;
    std::cout << "v3: 0x" << std::hex << static_cast<int>(reg[0x3]) << std::endl;
    std::cout << "v4: 0x" << std::hex << static_cast<int>(reg[0x4]) << std::endl;
    std::cout << "v5: 0x" << std::hex << static_cast<int>(reg[0x5]) << std::endl;
    std::cout << "v6: 0x" << std::hex << static_cast<int>(reg[0x6]) << std::endl;
    std::cout << "v7: 0x" << std::hex << static_cast<int>(reg[0x7]) << std::endl;
    std::cout << "v8: 0x" << std::hex << static_cast<int>(reg[0x8]) << std::endl;
    std::cout << "v9: 0x" << std::hex << static_cast<int>(reg[0x9]) << std::endl;
    std::cout << "vA: 0x" << std::hex << static_cast<int>(reg[0xA]) << std::endl;
    std::cout << "vB: 0x" << std::hex << static_cast<int>(reg[0xB]) << std::endl;
    std::cout << "vC: 0x" << std::hex << static_cast<int>(reg[0xC]) << std::endl;
    std::cout << "vD: 0x" << std::hex << static_cast<int>(reg[0xD]) << std::endl;
    std::cout << "vE: 0x" << std::hex << static_cast<int>(reg[0xE]) << std::endl;
    std::cout << "vF: 0x" << std::hex << static_cast<int>(reg[0xF]) << std::endl;

    std::cout << "==== Stack ====" << std::endl;
    for (int i=sizeof(stack)/2-1; i >= 0; i--) {
        std::cout << "0x" << std::hex << i << ": 0x" << std::hex << stack[i];
        if (i == sp) {
            std::cout << " <--";
        }

        std::cout << std::dec << std::endl;
    }
}

int main(int argc, char** argv) {
    int frameCount = 0;
    if (argc <= 1) {
        std::cout << "No argument specified" << std::endl;
        return 1;
    }

    std::cout << "Loading ROM " << argv[1] << std::endl;

    if (init(argv[1]) != 0) {
        std::cout << "Init failed" << std::endl;
        return 1;
    }

    printStatus();

    while (1) {
        // TODO update timers
        if (frameCount % 17 == 0) {
            if (delayTimer)
                delayTimer--;
            if (soundTimer)
                soundTimer--;
        }
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
            opCode.value = fetch(pc);
            pc += 2;
            execute(opCode);
        }

        if (updateScreen) {
            std::cout << "!!!! UPDATED SCREEN !!!!" << std::endl;
            IO::updateScreen();
            updateScreen = 0;
        }

        // Execute 1000 instructions per second
        frameCount++;
        IO::delay(1);//SDL_Delay( 1 );
    }

    return 0;
}

void opCallRCA(OpCode op) {
    // 0NNN Calls RCA 1802 program at address NNN
    // Ignored since it isn't compatible with most ROMs
    return;
}

void opClearScreen(OpCode op) {
    // 00E0 Clears the screen
    for (int x=0; x<CHIP8_SCREEN_WIDTH; x++) {
        for (int y=0; y<CHIP8_SCREEN_HEIGHT; y++) {
            pixels->data[x][y].r = 0;
            pixels->data[x][y].g = 0;
            pixels->data[x][y].b = 0;
        }
    }
    updateScreen = 1;
}

void opReturnSubroutine(OpCode op) {
    // 00EE
    // Returns from a subroutine
    pc = stack[sp--];
}

void opJump(OpCode op) {
    // 1NNN
    // Jumps to address NNN
    pc = op.nNN;
}

void opCallSubroutine(OpCode op) {
    // 2NNN
    // Puts the current pc at the top of the stack, then calls subroutine at NNN
    stack[++sp] = pc;
    pc = op.nNN;
}

void opSkipEq(OpCode op) {
    // 3XNN
    // Skip next instruction if vX equals NN
    if (reg[op.vX] == op.nN) {
        pc += 2;
    }
}

void opSkipNeq(OpCode op) {
    // 4XNN
    // Skips the next instruction if vX does not equal NN
    if (reg[op.vX] != op.nN) {
        pc += 2;
    }
}

void opSkipREq(OpCode op) {
    // 5XY0
    // Skips the next instruction if vX equals vY
    if (reg[op.vX] == reg[op.vY]) {
        pc += 2;
    }
}

void opMovConstant(OpCode op) {
    // 6XNN
    // Set VX to NN
    reg[op.vX] = op.nN;
}

void opAddConstant(OpCode op) {
    // 7XNN
    // Add NN to VX
    reg[op.vX] += op.nN;
}

void opMov(OpCode op) {
    // 8XY0
    // Set VX to VY
    reg[op.vX] = reg[op.vY];
}

void opOr(OpCode op) {
    // 8XY1
    // Set VX to VX or VY
    reg[op.vX] |= reg[op.vY];
}

void opAnd(OpCode op) {
    // 8XY2
    // Set VX to VX and VY
    reg[op.vX] &= reg[op.vY];
}

void opXor(OpCode op) {
    // 8XY3
    // Set VX to VX xor VY
    reg[op.vX] ^= reg[op.vY];
}

void opAdd(OpCode op) {
    // 0x8XY4
    // Set VX to VX plus VY, set F register to carry bit
    uint16 sum = reg[op.vX] + reg[op.vX];
    reg[op.vX] = sum;

    if (sum > 0xFF) {
        reg[0xF] = 1;
    } else {
        reg[0xF] = 0;
    }
}

void opSub(OpCode op) {
    // 8XY5
    // VX = VX - VY, set F register to NOT(borrow bit)
    if (reg[op.vX] > reg[op.vY]) {
        reg[0xF] = 1;
    } else {
        reg[0xF] = 0;
    }
    reg[op.vX] -= reg[op.vY];
}

void opRShift(OpCode op) {
    // 8XY6
    // Set VX to VX right shifted by 1, set F register to LSB
    reg[0xF] = reg[op.vX] & 0xF;
    reg[op.vX] >>= 1;
}

void opRSub(OpCode op) {
    // 8XY7
    // Set VX to VY minus VX, set F register to borrow bit
    if (reg[op.vY] > reg[op.vX]) {
        reg[0xF] = 1;
    } else {
        reg[0xF] = 0;
    }
    reg[op.vX] = reg[op.vY] - reg[op.vX];
}

void opLShift(OpCode op) {
    // 8XYE
    // Set VX to VX left shifted by 1, set F register to MSB
    reg[0xF] = reg[op.vX] & 0x80;
    reg[op.vX] <<= 1;
}

void opSkipRNeq(OpCode op) {
    // 9XY0
    // Skips the next instruction if vX does not equal vY
    if (reg[op.vX] != reg[op.vY]) {
        pc += 2;
    }
}

void opSetIndex(OpCode op) {
    // ANNN
    // Set the index address to NNN
    iAddress = op.nNN;
}

void opJumpOffset(OpCode op) {
    // BNNN
    // Set the program counter to V0 plus NNN
    pc = reg[0x0] + op.nNN;
}

void opRandAnd(OpCode op) {
    // CXNN
    // Set VX to a random number masked by NN
    reg[op.vX] = IO::random() & op.nN;
}

void opDraw(OpCode op) {
    // 0xDXYN
    reg[0xF] = 0;
    for (int y = 0; y < op.n; y++) {
        for (int x = 0; x < 8; x++) {
            if ( !((0x80 >> x) & ram[iAddress+y]) ) {
                // Pixel at XY is black, no drawing
                continue;
            } else {
                //Need to draw
                uint8 xPos = (reg[op.vX] + x) % CHIP8_SCREEN_WIDTH;
                uint8 yPos = (reg[op.vY] + y) % CHIP8_SCREEN_HEIGHT;
                if (pixels->data[xPos][yPos].r) {
                    // If we are drawing over a white pixel there is a collision
                    reg[0xF] = 1;
                }

                pixels->data[xPos][yPos].r ^= 0xFF;
                pixels->data[xPos][yPos].g ^= 0xFF;
                pixels->data[xPos][yPos].b ^= 0xFF;
            }
        }
    }
    updateScreen = 1;
}

void opSkipWhenKeyDown(OpCode op) {
    if ( (0x8000 >> op.vX) & keyboard.keys) {
        pc += 2;
    }
}

void opSkipWhenKeyUp(OpCode op) {
    if (!((0x8000 >> op.vX) & keyboard.keys)) {
        pc += 2;
    }
}

void opGetDelayTimer(OpCode op) {
    // FX07
    // Set VX to the value in the delay timer
    reg[op.vX] = delayTimer;
}

void opWaitForKeyPress(OpCode op) {
    // FX0A
    // Pause execution until a key press is received
}

void opSetDelayTimer(OpCode op) {
    // FX15
    // Set the delay timer to vX
    delayTimer = reg[op.vX];
}

void opSetSoundTimer(OpCode op) {
    // FX18
    // Set the sound timer to vX
    soundTimer = reg[op.vX];
}

void opAddToIndex(OpCode op) {
    // FX1E
    // Adds vX to the index address
    iAddress += reg[op.vX];
}

void opSetIndexToCh(OpCode op) {
    // FX29
    // Sets address I to the location where the sprite for the character for the
    // value in vX is stored
    iAddress = ram[op.vX*FONT_HEIGHT + FONT_OFFSET];
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

void opUnknown(OpCode op) {
    std::cout << "Unknown OpCode: " << op.value << std::endl;
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
