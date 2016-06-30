#include "input_adapter.h"

void HexKeyboardAdapter::updateAdapter(SDL_Keycode keyCode, uint8 state) {
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

HexKeyboardAdapter::HexKeyboardAdapter() {
    status.keys = 0;
}

HexKeyboardAdapter::~HexKeyboardAdapter() {}
HexKeyboardStatus HexKeyboardAdapter::getStatus() {
    getUpdates();
    return status;
}

