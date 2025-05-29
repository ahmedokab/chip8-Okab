class chip8 {
    public:
        void initialize();
        void loadGame(const char* fileName);
        void emulateCycle();
        void SetKeys();

        bool drawFlag;
        unsigned char gfx[64 * 32]; // the graphics of the Chip 8 are black and white
        // the screen has a total of 2048 pixels (64 x 32). This can be implemented using an array that holds the pixel state, either a 1 or a 0.
    private:
        unsigned short opcode;
        unsigned char memory[4096]; //4096 memory locations
        unsigned char V[16]; // the 16 registers
        unsigned short I; // 12 bit address register used w/ many operaations that involve memory operations
        unsigned short pc;//value from 0x000 to 0xFFF
        unsigned char delay_timer;
        unsigned char sound_timer; // There are two timer registers that count at 60hz
        //when set above 0, they count down to 0
        unsigned short stack[16];
        unsigned short sp; // this is the stack pointer
    
}