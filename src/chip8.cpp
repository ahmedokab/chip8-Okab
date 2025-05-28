#include "chip8.h"
#include <iostream>
#include <fstream>   // for loading ROM
#include <cstring>   // for memset, etc.

// --- Fontset (fill in later or copy from guide) ---
unsigned char chip8_fontset[80] = {
    // TODO: Fill with actual CHIP-8 fontset data (each character = 5 bytes)
};

// --- CHIP-8 Initialization ---
void Chip8::initialize() {
    // TODO: Reset memory, registers, PC, stack, timers, load fontset
}

// --- Load ROM into memory ---
void Chip8::loadGame(const char* filename) {
    // TODO: Open binary file and load it into memory[0x200...]
}

// --- Emulate One CPU Cycle ---
void Chip8::emulateCycle() {
    // TODO: Fetch, decode, execute one opcode
    // TODO: Update timers
}

// --- Update Key Input ---
void Chip8::setKeys() {
    // Optional: Implement only if input is handled inside Chip8 class
}
