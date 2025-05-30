#include <iostream>
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include "chip8.h"
#include "tinyfiledialogs.h"

chip8 myChip8;

// Graphics settings
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
const int WINDOW_SCALE = 10;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;
const int cycles_perframe = 15;

const SDL_Keycode keymap[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};



bool setupGraphics(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        SDL_Log("Could not initialize SDL subsystems %s\n" , SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("CHIP-8 Emulator",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH * WINDOW_SCALE,
                              SCREEN_HEIGHT * WINDOW_SCALE,
                              SDL_WINDOW_SHOWN);
    if(!window){
        SDL_Log("Could not create window %s\n" , SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        SDL_Log("Could not make renderer: %s", SDL_GetError());
        return false;
    }


    return true;

};     // Initializes SDL2 window and renderer
void drawGraphics(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Creatning a black background
    SDL_RenderClear(renderer);

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (myChip8.gfx[y * SCREEN_WIDTH + x]) {
                SDL_Rect pixel = {
                    x * WINDOW_SCALE,
                    y * WINDOW_SCALE,
                    WINDOW_SCALE,
                    WINDOW_SCALE
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White pixel
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
    myChip8.drawFlag = false;

};      // Renders gfx[] array to screen
void setKeys(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool isPressed = event.type == SDL_KEYDOWN;

            for (int i = 0; i < 16; ++i) {
                if (event.key.keysym.sym == keymap[i]) {
                    myChip8.key[i] = isPressed;
                }
            }
        }
    }    
    
};           // Updates key[] array based on SDL input

int main(int argc, char **argv){

    //setting up render system and register input callbacks
    if (!setupGraphics()){
        return 1;
    }; //initializes SDL2, creates a window 640x32 and a renderer for drawing pixels


    // initialze the chip8 system and load game into memory
    myChip8.initialize(); //powers up virtual chip-9 console by clearing memory, changingpc, resetting registers etc.


        const char *filter_patterns[1] = { "*.ch8" };
    const char *romPath = tinyfd_openFileDialog(
        "Choose a CHIP-8 ROM! Project by Ahmed Okab",  // title
        "",                     // default path
        1,                      // number of filters
        filter_patterns,     
        "CHIP-8 ROMs",          
        0                       //  multiple select?
    );

    if (!romPath) {
        std::cout << "No ROM selected.\n"; // no rom selected if path just doesnt exist
        return 1;
    }

        myChip8.loadGame(romPath); //LOAD

    while(true){
        for(int i = 0; i < cycles_perframe; ++i){
            myChip8.emulateCycle();
            setKeys();// if we press or release a key, we should store this in a part that emulates the keypad
        }

        if (myChip8.drawFlag){ // 0x00E0 – Clears the screen
            // 0xDXYN – Draws a sprite on the screen
            drawGraphics();
        }
        if(myChip8.delay_timer > 0)
            --myChip8.delay_timer;
 
        if(myChip8.sound_timer > 0)
        {
        if(myChip8.sound_timer == 1) std::cout << "BEEP\n";  
        --myChip8.sound_timer;
        } 

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}