#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"

chip8 myChip8;

// Graphics settings
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
const int WINDOW_SCALE = 10;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;


void setupGraphics(){

};     // Initializes SDL2 window and renderer
void drawGraphics(){

};      // Renders gfx[] array to screen
void setKeys(){
    
};           // Updates key[] array based on SDL input

int main(int argc, char **argv){

    //setting up render system and register input callbacks
    setupGraphics(); //initializes SDL2, creates a window 640x32 and a renderer for drawing pixels
    setupInput();


    // initialze the chip8 system and load game into memory
    myChip8.initialize(); //powers up virtual chip-9 console by clearing memory, changingpc, resetting registers etc.
    myChip8.loadGame("pong"); //loads rom file into x200 to be executed

    while(true){
        for(int i = 0; i < cycles_perframe; ++i){
            myChip8.emulateCycle();
        }

        if (myChip8.drawFlag){ // 0x00E0 – Clears the screen
            // 0xDXYN – Draws a sprite on the screen
            drawGraphics();
        }

        setKeys();  // if we press or release a key, we should store this in a part that emulates the keypad

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}