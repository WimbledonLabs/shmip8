#ifndef SHM_EMU_IO
#define SHM_EMU_IO

#include "shmip8_common.h"
#include <SDL2/SDL.h>

namespace IO {
    void initialize(void);
 
    class InputAdapter {
    public:
        InputAdapter();
        ~InputAdapter();
        void updateAdapter(SDL_Keycode, uint8);
        void getUpdates(void);
    };

    class Screen {
    public:
        Screen(uint32 width, uint32 height, float scale);
        ~Screen();
        void updateScreen(uint32* data);
    private:
        SDL_Window *m_win = nullptr;
        SDL_Renderer *m_ren = nullptr;
        SDL_Texture *m_tex = nullptr;
    };

}

#endif // SHM_EMU_IO
