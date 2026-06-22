# CHIP-8 Emulator

A CHIP-8 emulator written in C++ using SDL2 for graphics and input handling.

This project was built as a deep dive into low-level programming concepts such as CPU emulation, memory management, instruction decoding, timers, and event-driven graphics rendering.

## Features

* Complete CHIP-8 instruction set implementation
* 4 KB memory model
* 16 general-purpose registers (V0–VF)
* Stack and subroutine support
* Delay and sound timers
* Sprite-based graphics rendering (64×32 display)
* SDL2 window and rendering system
* CHIP-8 keypad input support
* Fontset loading and sprite rendering
* ROM loading from file
* Corax89 opcode test ROM compatibility
* Successfully runs classic CHIP-8 games such as Pong

## Controls

The CHIP-8 keypad is mapped to the keyboard as follows:

| CHIP-8  | Keyboard |
| ------- | -------- |
| 1 2 3 C | 1 2 3 4  |
| 4 5 6 D | Q W E R  |
| 7 8 9 E | A S D F  |
| A 0 B F | Z X C V  |

## Technical Overview

The emulator implements the complete CHIP-8 fetch-decode-execute cycle:

1. Fetch instruction from memory using the program counter.
2. Decode opcode and operands.
3. Execute instruction.
4. Update timers.
5. Render graphics when the display changes.
6. Process keyboard input.

Major components include:

* CPU emulation
* Memory subsystem
* Stack management
* Timer subsystem
* Graphics renderer
* Input handling
* ROM loader

## Example ROMs

Tested with:

* Corax89 Opcode Test ROM
* Pong

## Building

### Requirements

* C++17 compatible compiler
* SDL2
* CMake 3.15+

### Build

```bash
git clone <repository-url>
cd chip8

mkdir build
cd build

cmake ..
cmake --build .
```

## Running

```bash
./chip8
```

Load a CHIP-8 ROM and enjoy.

## What I Learned

This project was built to better understand:

* CPU architecture fundamentals
* Instruction decoding
* Bitwise operations
* Memory and stack management
* Timers and event loops
* SDL2 graphics and input
* Emulator architecture and debugging

## Future Improvements

* Sound output
* Additional compatibility quirks
* Super-CHIP support
* Debugger and disassembler
* Save states
* Configurable key mappings

## Screenshots
<img width="802" height="377" alt="image" src="https://github.com/user-attachments/assets/c833ae2d-99b0-49f1-929a-e784eaa0546d" />
<img width="796" height="406" alt="image" src="https://github.com/user-attachments/assets/7cd2d0b9-5952-4592-a4af-32daff43bff6" />



Built in C++ and SDL2 as a systems programming learning project.
