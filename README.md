# shmip8
shmip8 is a basic CHIP-8 emulator written in C++. It is a quick and simple implementation I created to get my feet wet in emulation. This project should be called a "virtual machine" rather than an "emulator" though, since the CHIP-8 never existed as physical hardware, and only existed as an interpreter. Since the implementation of a CHIP-8 interpreter is similar to that of an emulator, it is commonly called an emulator anyways.

![Image of Chip-8 Tetris](https://github.com/WimbledonLabs/shmip8/raw/master/raw/tetris.png)
![Image of Chip-8 Pac-Man](https://github.com/WimbledonLabs/shmip8/raw/master/raw/blinky.png)

## About the CHIP-8
The original CHIP-8 interpreter and programming language were created in the 1970's for the 8-bit microprocessors of the day. It was designed to make it easier to program video games on those computers.

CHIP-8 programs have access to:
* 64x32 pixel monochrome display
* 3232 bytes for program memory
* a buzzer
* 15 general purpose registers, and one flag register
* a timer for sound and another for general time-keeping
* a stack which supports 12 layers of nesting
* 4x4 hex keyboard

## Games
[David Winter has created many classic games to run on the CHIP-8.](http://www.pong-story.com/chip8/) These games include Tetris, Pong, Breakout, Minesweeper etc.
