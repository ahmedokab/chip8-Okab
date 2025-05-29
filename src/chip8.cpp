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
    pc     = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode	
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer
    
    // Clear display	
    // Clear stack
    // Clear registers V0-VF
    // Clear memory
    
    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];		
    
    // Reset timers
    
}

// --- Load ROM into memory ---
void Chip8::loadGame(const char* filename) {
    // TODO: Open binary file and load it into memory[0x200...]
}

// --- Emulate One CPU Cycle ---
void Chip8::emulateCycle() {
    // TODO: Fetch, decode, execute one opcode
    // to implement fetch
    opcode = memory[pc] << 8 | memory[pc + 1]; // in order to merge both bytes and store them in unsigned short(2 bytes datatype), 
    // using bitwise OR operation to merge

    //implementing decode, decode the opcode adn check the opcode table to see what it means
    
    switch(opcode & 0xF000) //Bitmask only keeps the first 4 bits of the current opcode
    {
        //opcodes
        case 0x0000:
                switch(opcode & 0x000F)
                {
                    case 0x0000: // 0x00E0: Clears the screen        
                        for(int i = 0; i < 2048; ++i)
                            gfx[i] = 0x0;
                        drawFlag = true;
                        pc += 2; 
                    break;
                
                    case 0x000E: // 0x00EE: Returns from subroutine          
                        // Execute opcode
                        --sp;
                        pc = stack[sp]; //program counter 
                        pc += 2; //increases program counter by two lccations
                    break;
                
                default:
                    printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
                }
            break;
                
        case 0xA000: // ANNN: sets I to the address NNN
            //executing the opcode
            I = opcode & 0x0FFF;
            pc += 2;
        break;
        
        case 0x1000: //1NNN - Jumps to address NNN
            pc = opcode & 0x0FFF;
        break;
        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
        break;
        case 0x3000:
                if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                    pc += 4; // if equal skips next instruction
                } //rightshifts by 8 to make it 4 LSB on the right
                else{
                    pc += 2;
                }
        break;
        case 0x4000:
                if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                    pc += 4; // if not equal skips next instruction
                } //rightshifts by 8 to make it occupy 4 LSB on the right, then checks it against NN
                else{
                    pc += 2;
                }
        break;
        case 0x5000:
                if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                    pc += 4; // if Vx and Vy equal skips next instruction
                } // (usually the next instruction is a jump to skip a code block)
                else{
                    pc += 2;
                }
        break;


        default:
            printf("Unknown opcode: 0x%X\n", opcode);




    }

    // TODO: Update timers
      if(delay_timer > 0)
        --delay_timer;
 
  if(sound_timer > 0)
  {
    if(sound_timer == 1)
      printf("BEEP!\n");
    --sound_timer;
  }  

}

// --- Update Key Input ---
void Chip8::setKeys() {
    // Optional: Implement only if input is handled inside Chip8 class
}
