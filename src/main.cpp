#include <iostream>  // std::cout, std::endl, std::hex
#include <fstream>   // std::ifstream

#include <iomanip>
#include <bitset>

#include "shmip8_core.h"
#include "shm_emu_io.h"

#define DEFAULT_WINDOW_SCALE 10

const float SCREEN_FPS = 60.0;
const float TICKS_PER_SECOND = 1000.0;
const float TICKS_PER_FRAME = TICKS_PER_SECOND / SCREEN_FPS;

class HexKeyboardAdapter : IO::InputAdapter {
    private:
        HexKeyboardStatus status;

        void updateAdapter(SDL_Keycode keyCode, uint8 state) {
            switch (keyCode) {
            case SDLK_a:
                status.k0 = state;
                break;
            case SDLK_1:
                status.k1 = state;
                break;
            case SDLK_s:
                status.k2 = state;
                break;
            case SDLK_4:
                status.k3 = state;
                break;
            case SDLK_q:
                status.k4 = state;
                break;
            case SDLK_w:
                status.k5 = state;
                break;
            case SDLK_e:
                status.k6 = state;
                break;
            case SDLK_r:
                status.k7 = state;
                break;
            case SDLK_2:
                status.k8 = state;
                break;
            case SDLK_3:
                status.k9 = state;
                break;
            case SDLK_d:
                status.kA = state;
                break;
            case SDLK_f:
                status.kB = state;
                break;
            case SDLK_z:
                status.kC = state;
                break;
            case SDLK_x:
                status.kD = state;
                break;
            case SDLK_c:
                status.kE = state;
                break;
            case SDLK_v:
                status.kF = state;
                break;
            default:
                break;
            }
        }

    public:
        HexKeyboardAdapter() {}
        ~HexKeyboardAdapter() {}
        HexKeyboardStatus getStatus() {
            getUpdates();
            return status;
        }

};

int main(int argc, char** argv) {
    if (argc <= 1) {
        std::cout << "Must specify path to ROM" << std::endl;
        exit(1);
    }

    Chip8::Core core;

    HexKeyboardAdapter keyboardAdapter;

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

    /*for (int i=0x200; i<0x200+256; i+=2) {
        std::cout << std::hex << i << ": "<< std::setw(2) << std::hex << (int) core.ram[i] << std::setw(2) << std::hex << (int) core.ram[i+1] << std::endl;
    }*/

    uint32 timeSinceLastUpdate = SDL_GetTicks();
    double tickCounter = 0;

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
        
        core.step();

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

void printOp(Chip8::Core *core, OpCode op) {
    std::cout << std::setw(4) << std::hex << core->pc << ": " << std::setw(4) << std::hex << op.value << std::endl;
}
