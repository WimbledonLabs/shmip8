#include <iostream>  // std::cout, std::endl, std::hex
#include <fstream>   // std::ifstream

#include <iomanip>
#include <bitset>

#include "shmip8_core.h"
#include "shm_emu_io.h"
#include "input_adapter.h"

#define DEFAULT_WINDOW_SCALE 10

const float SCREEN_FPS = 60.0;
const float TICKS_PER_SECOND = 1000.0;
const float TICKS_PER_FRAME = TICKS_PER_SECOND / SCREEN_FPS;

void printInstruction(OpCode op);
void printStateInfo(Chip8::Core *core, OpCode op);

int main(int argc, char** argv) {
    uint32 timeSinceLastUpdate = SDL_GetTicks();
    double tickCounter = 0;

    Chip8::Core core;
    HexKeyboardAdapter keyboardAdapter;

    if (argc <= 1) {
        std::cout << "Must specify path to ROM" << std::endl;
        exit(1);
    }

    IO::initialize();
    IO::Screen display(core.displayWidth,
                       core.displayHeight, 
                       DEFAULT_WINDOW_SCALE);

    std::ifstream romFile(argv[1], std::ifstream::in);

    if (!romFile.is_open()) {
        std::cout << "Error: Opening ROM failed" << std::endl;
        exit(1);
    }

    core.loadROM(&romFile);

    while (1) {
        // TODO don't update time when window isn't focused
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > timeSinceLastUpdate) {
            // Update time must be non-zero
            Uint32 elapsedTime =  currentTime - timeSinceLastUpdate;
            tickCounter += elapsedTime; 

            // Update timers, generate interrupts etc (no proccessing)
            core.timeStep(elapsedTime);
            timeSinceLastUpdate = currentTime;
        }

        if (tickCounter > TICKS_PER_FRAME && core.screenUpdateAvailable()) {
            display.updateScreen(core.getPixelBuffer());
            tickCounter -= TICKS_PER_FRAME;
        }

        core.updateInput(keyboardAdapter.getStatus());

        #if DEBUG_MODE >= 1
            OpCode inOp;
            inOp.value = core.fetch();
            std::cout << std::hex << ((int) core.pc) << ": ";
            printInstruction(inOp);
            std::cout << std::endl;
        #endif

        #if DEBUG_MODE >= 2
            printStateInfo(&core, inOp);
            std::cout << std::endl;
        #endif

        core.step();

        #if DEBUG_MODE >= 2
            printStateInfo(&core, inOp);
            std::cout << std::endl;
        #endif

    }

    return 0;
}

void printStatus(Chip8::Core *core) {
    std::cout << "Current Instruction: 0x" << std::hex << (core->opCode.value) << std::endl;

    std::cout << "v0: 0x" << std::hex << static_cast<int>(core->reg[0x0]) << std::endl;
    std::cout << "v1: 0x" << std::hex << static_cast<int>(core->reg[0x1]) << std::endl;
    std::cout << "v2: 0x" << std::hex << static_cast<int>(core->reg[0x2]) << std::endl;
    std::cout << "v3: 0x" << std::hex << static_cast<int>(core->reg[0x3]) << std::endl;
    std::cout << "v4: 0x" << std::hex << static_cast<int>(core->reg[0x4]) << std::endl;
    std::cout << "v5: 0x" << std::hex << static_cast<int>(core->reg[0x5]) << std::endl;
    std::cout << "v6: 0x" << std::hex << static_cast<int>(core->reg[0x6]) << std::endl;
    std::cout << "v7: 0x" << std::hex << static_cast<int>(core->reg[0x7]) << std::endl;
    std::cout << "v8: 0x" << std::hex << static_cast<int>(core->reg[0x8]) << std::endl;
    std::cout << "v9: 0x" << std::hex << static_cast<int>(core->reg[0x9]) << std::endl;
    std::cout << "vA: 0x" << std::hex << static_cast<int>(core->reg[0xA]) << std::endl;
    std::cout << "vB: 0x" << std::hex << static_cast<int>(core->reg[0xB]) << std::endl;
    std::cout << "vC: 0x" << std::hex << static_cast<int>(core->reg[0xC]) << std::endl;
    std::cout << "vD: 0x" << std::hex << static_cast<int>(core->reg[0xD]) << std::endl;
    std::cout << "vE: 0x" << std::hex << static_cast<int>(core->reg[0xE]) << std::endl;
    std::cout << "vF: 0x" << std::hex << static_cast<int>(core->reg[0xF]) << std::endl;

    std::cout << "==== Stack ====" << std::endl;
    for (int i=sizeof(core->stack)/2-1; i >= 0; i--) {
        std::cout << "0x" << std::hex << i << ": 0x" << std::hex << core->stack[i];
        if (i == core->sp) {
            std::cout << " <--";
        }

        std::cout << std::dec << std::endl;
    }
}

void printKeyboard(Chip8::Core *core) {
    std::bitset<16> x(core->keyboard.keys);
    std::cout<< x << std::endl;
}

void printInstruction(OpCode op) {
    #include "disassembleSwitch.cpp"
}

int BCD(Chip8::Core *core, OpCode op) {
    return 0;
}

int ramUpToVX(Chip8::Core *core, OpCode op) {
    return 0;
}

int regUpToVX(Chip8::Core *core, OpCode op) {
    return 0;
}

void printStateInfo(Chip8::Core *core, OpCode op) {
    #include "stateInfoSwitch.cpp"
}
