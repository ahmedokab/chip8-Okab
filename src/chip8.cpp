#include "chip8.h"
#include <iostream>
#include <fstream>   // for loading ROM
#include <cstring>   // for memset, etc.
#include <ctime>
#include <cstdlib>



// --- Fontset (fill in later or copy from guide) ---
unsigned char chip8_fontset[80] = {
    // TODO: Fill with actual CHIP-8 fontset data (each character = 5 bytes)
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// --- CHIP-8 Initialization ---
void chip8::initialize() {
    // TODO: Reset memory, registers, PC, stack, timers, load fontset
    std::srand(std::time(nullptr));
    pc     = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode	
    I      = 0;      // Reset index register
    sp     = 0;      // Reset stack pointer
    
    // Clear display	
    for(int i = 0; i < (64 * 32); ++i){
        gfx[i] = 0; //clearing the display
    }

    // Clear stack

    for (int i = 0; i < 16; ++i) {
        stack[i] = 0;
    }

    // Clear registers V0-VF

    for (int i = 0; i < 16; ++i) {
        V[i] = 0;
    }

    // Clear memory
    for (int i = 0; i < 4096; ++i) {
        memory[i] = 0;
    }

    
    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];		
    
    // Reset timers
    
}

// --- Load ROM into memory ---
void chip8::loadGame(const char* filename) {
    const unsigned short START_ADDRESS = 0x200; //loads the ROM at the address starting from x200

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open ROM: " << filename << std::endl;
        exit(1);
    }

    std::streamsize romsize = file.tellg(); //tellg tells us current file pointer location, this gives us the romsize because std::ios::ate is at the end so gives us romsize
    if (romsize > (4096 - START_ADDRESS)) {
        std::cerr << "ROM too large to fit in memory." << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::beg); //moves file pointer back to the beginning
    file.read(reinterpret_cast<char*>(&memory[START_ADDRESS]), romsize); //reading from the file into the memory from the starting address byte by byte until the end of the rom
    file.close();

    std::cout << "Loaded ROM: " << filename << " (" << romsize << " bytes)" << std::endl;
}


// --- Emulate One CPU Cycle ---
void chip8::emulateCycle() {
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
                    printf ("Unknown opcode: 0x%X\n", opcode);          
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
        case 0x6000:
                V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
                pc += 2;
        break;
        case 0x7000:
                V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
                pc += 2;
        break;
        case 0x8000:
                switch(opcode & 0x000F){
                    case 0x0:
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                        pc += 2; 
                    break;

                    case 0x1:
                        V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                        pc += 2; 
                    break;

                    case 0x2:
                        V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                        pc += 2; 
                    break;

                    
                    case 0x3:
                        V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                        pc += 2; 
                    break;

                    case 0x4: // VY += VX
                    //  VF is set to 1 when there is a carry and set to 0 when there isn't. Register only holds value from 0 to 255, it can't be stored in the register
                    // starts counting from 0 again. So if sum of VX and VY is greater than 255, we use carry flag 
                        if (V[(opcode & 0x0F00) >> 8] > 0xFF - V[(opcode & 0x00F0) >> 4] ){
                            V[0xF] = 1;
                        } // this means 255 - Vx < Vy so Vy + Vx > 255 so overflow
                        else{
                            V[0xF] = 0;
                        }
                        V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                        pc += 2;
                        //increment program counter by 2
                    break;
                    case 0x5:
                        if (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4] ){
                            V[0xF] = 1;
                        }
                        else{
                            V[0xF] = 0;
                        }
                        V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                        pc += 2;
                    break;

                    case 0x6:
                        V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x0001;
                        V[(opcode & 0x0F00) >> 8] >>= 1;
                        pc += 2;
                    break;

                    case 0x7:
                        if (V[(opcode & 0x0F00) >> 8] <= V[(opcode & 0x00F0) >> 4] ){
                            V[0xF] = 1;
                        } // VY >= VX means VF is set to 1 as underflow
                        else{
                            V[0xF] = 0; //else set to 0
                        }
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8]; // Vx = Vy - Vx
                    break;

                    case 0xE:
                        V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
                        V[(opcode & 0x0F00) >> 8] <<= 1;
                        pc += 2;
                    break;                

                    default:
                        printf ("Unknown opcode: 0x%X\n", opcode);
                }
        break;

        case 0x9000:
                if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                    pc += 4;
                }
                else{
                    pc += 2;
                }
        break;

        case 0xB000:
                pc = V[0x0] + (opcode & 0xFFF);
        break;

        case 0xC000:
            {
                unsigned char random_number = std::rand() % 256;
                V[(opcode & 0x0F00) >> 8] = random_number & (opcode & 0x00FF);
                pc += 2;
                break; }


        case 0xD000://draw command DXYN 
        // Vx is X coord
        // Vy is Y coord
        // N is number of bytes/rows to draw, height of sprite
        //sprite is always 8 bits wide, comes from memory address starting at address I
        {
        unsigned char x = V[(opcode & 0x0F00) >> 8];
        unsigned char y = V[(opcode & 0x00F0) >> 4];
        unsigned char height = opcode & 0x000F;
        unsigned char pixel;

        V[0xF] = 0; //resetting V[F], the collision flag

        for(int h = 0; h < height; h++){ //going row by row for the height of the sprite
            pixel = memory[I + h]; //reads that single row of the spite
            for(int bit = 0; bit < 8; bit++){
                if((pixel & (0x80 >> bit)) != 0){ //checks if that current bit in the row we are checking is a 1, by bitmasking it. If it is, then we draw
                    int drawIndex = ((x+bit)) + (((y + h) * 64)); // using row major order i + j * nc  to get access index
                    if (gfx[drawIndex] == 1) {
                        V[0xF] = 1; // Pixel was on and now will be turned off. There was a collision
                    }
                    gfx[drawIndex] ^= 1; //toggle pixel, was off then turns on. Or was off then turns on
                }

            }
            

        }
        drawFlag = true; // flag triggered so screen will be redrawn. We changed our gfx array and thus need to update the screen
        pc += 2; // move to next opcode
        break;  }

        case 0xE000:
            switch(opcode & 0x000F){
                case 0xE: //skips the next instruction if the key stored in Vx is pressed
                  if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                    pc += 4;
                  else
                    pc += 2;
                break;

                case 0x1:
                if(key[V[(opcode & 0x0F00) >> 8]] == 0)
                    pc += 4;
                  else
                    pc += 2;
                break;
            }
        break;

        case 0xF000:
            switch(opcode & 0x0FF){
                case 0x07: 
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break;

                case 0x0A:{
                    // waiting for a key press then storing the value of the key in Vx
                    bool pressed = false;
                    
                    for(int i = 0; i < 16; ++i){
                        if (key[i] != 0){
                            V[(opcode & 0x0F00) >> 8] = i;  //Vx = key index that has been pressed
                            pressed = true;
                            break;
                        }
                    }

                    if (!pressed){
                        return; // wait for the key to be pressed, do not advance pc

                    }

                    pc += 2; //advance program counter if it was pressed
                } break;

                case 0x15:
                    delay_timer =  V[(opcode & 0x0F00) >> 8];
                    pc += 2; //setting delaytimer to Vx
                break;

                case 0x18:
                    sound_timer =  V[(opcode & 0x0F00) >> 8];
                    pc += 2; //setting soundtimer to Vx
                break;

                case 0x1E: //memory operation, adds vx to I
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x29:
                {
                    // FX29: Set I = location of sprite for digit Vx
                    unsigned char digit = V[(opcode & 0x0F00) >> 8];  // get the digit (0x0 - 0xF)
                    I = digit * 5;  // Each character sprite is 5 bytes long
                    pc += 2;
                    break;
                }



                case 0x33:
                {
                    unsigned char value = V[(opcode & 0x0F00) >> 8];
                    unsigned char hundreds = 0;
                    unsigned char tens = 0;
                    unsigned char ones = 0;

                    // Count hundreds
                    while (value >= 100) {
                        value -= 100;
                        ++hundreds;
                    }

                    // Count tens
                    while (value >= 10) {
                        value -= 10;
                        ++tens;
                    }

                    // Remaining is ones
                    ones = value;

                    memory[I]     = hundreds;
                    memory[I + 1] = tens;
                    memory[I + 2] = ones;
                    pc += 2;
                
                break;
                }
            
                case 0x55:
                    {
                        unsigned char loopval = ((opcode & 0x0F00) >> 8);

                        for(int i = 0; i <= loopval; ++i){
                            memory[I + i] = V[i];
                        }
                        pc += 2;
                        break;
                    }



                case 0x65:
                        {
                        unsigned char loopval = ((opcode & 0x0F00) >> 8);

                        for(int i = 0; i <= loopval; ++i){
                           V[i] = memory[I + i];
                        }
                        pc += 2;
                        break;
                    }


                break;

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
    if(sound_timer == 1) std::cout << "BEEP\n";  
    --sound_timer;
  }  

}

