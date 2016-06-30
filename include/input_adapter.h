#include "shm_emu_io.h"

class HexKeyboardAdapter : IO::InputAdapter {
    private:
        HexKeyboardStatus status;
        void updateAdapter(SDL_Keycode keyCode, uint8 state);

    public:
        HexKeyboardAdapter();
        ~HexKeyboardAdapter();
        HexKeyboardStatus getStatus();
};


