# 🎮 CHIP-8 Emulator by Ahmed Okab

Have you ever wondered what goes on under the hood in a gaming console? Well, I did, and so I decided to recreate the Chip-8, an interpreted programming language from the mid 1970's, able to run classics like Pong, Space Invaders, Tetris & many more games!

In the process, I developed my lower-level programming skills to recreate a CPU's fetch-decode-execute cycle using opcode handling in C, all of which was handled by an infinite while(true) loop. The loop ran a large emulateCycle() many times per frame (which I had to experiment to find). This function served as the base of my program, with a switch-case statement handling each opcode, utilizing my skills of bit-masking, bit shifting and lower level concepts to decode the opcode and manipulating the program counter as well as other memory locations differently for each instruction.

## 🛠️ Features

- ✅ Opcode decoding for all 35 official CHIP-8 instructions
- ✅ Cycle-based emulation loop (timing matters!)
- ✅ Input handling via SDL2 with keys mapped from the original CHIP-8 to another input system (details below)
- ✅ Graphics rendering using XOR sprites - providing a flicker present in the old Chip-8
- ✅ Loads and runs public domain CHIP-8 ROMs flawlessly

## 🛠️ INPUT
| CHIP-8 Key | Mapped To |
|------------|-----------|
| 0x0        | X         |
| 0x1        | 1         |
| 0x2        | 2         |
| 0x3        | 3         |
| 0x4        | Q         |
| 0x5        | W         |
| 0x6        | E         |
| 0x7        | A         |
| 0x8        | S         |
| 0x9        | D         |
| 0xA        | Z         |
| 0xB        | C         |
| 0xC        | 4         |
| 0xD        | R         |
| 0xE        | F         |
| 0xF        | V         |


### 🔧 Requirements

- C++17 or higher
- SDL2 development libraries
- Make / g++ (or any compiler of your choice)

### 🛠️ Build Instructions
Use the make instruction, then run the file with this command!

make
./chip8 path/to/rom.ch8
