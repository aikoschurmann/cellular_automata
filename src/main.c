#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

const int  GRID_WIDTH = 400; // Define GRID_WIDTH and GRID_HEIGHT appropriately
const int GRID_HEIGHT = GRID_WIDTH; // Define GRID_HEIGHT

// Constants for window dimensions based on grid dimensions and cell size
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE; // Define GRID_WIDTH and GRID_HEIGHT appropriately
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE; // Define GRID_HEIGHT
int delay_ms = 100; // Initial delay (in milliseconds)
int iterations = 0;
int save_frequency = 20;
int states = 8;
char filename[100] = "./data/grid.txt";



int main(int argc, char const *argv[])
{
    // Initialize the window with the specified dimensions
    initialize_window("Automaton", WINDOW_WIDTH, WINDOW_HEIGHT);
    initialize_keyboard_state();

    struct grid grid;
    //struct color start = {255, 182, 193}; // Light Coral
    //struct color end = {135, 206, 250};   // Light Sky Blue

    // Alternatively, you can use these different sets of colors
    //struct color start = {144, 238, 144}; // Light Green
    //struct color end = {173, 216, 230};   // Light Blue

    // struct color start = {255, 218, 185}; // Peach
    // struct color end = {230, 230, 250};   // Lavender

    struct color start = {255, 228, 196}; // Light Beige
    struct color end = {139, 143, 67};   // Pale Olive Green

    //struct color start = {224, 255, 255}; // Light Cyan
    //struct color end = {176, 224, 230};   // Light Steel Blue

    //struct color start = {0, 0, 0};         // Black
    //struct color end = {255, 255, 255}; // White
    

    struct color* pallete = mallocpalette(states);

    initialize_gradient_palette(pallete, &start, &end, states);
    initialize_grid(&grid, GRID_WIDTH, GRID_HEIGHT, states, pallete); 

    int paused = 0;

    while (should_continue) {
        handle_events();

        if (is_key_down(SDL_SCANCODE_P)) {
            paused = !paused;  // Toggle pause
        }

        if (is_key_down(SDL_SCANCODE_ESCAPE)) {
            should_continue = 0; // Exit the loop
        }

        if (is_key_down(SDL_SCANCODE_UP)) {
            delay_ms = (delay_ms > 10) ? delay_ms - 10 : delay_ms; // Increase speed
        }
        if (is_key_down(SDL_SCANCODE_DOWN)) {
            delay_ms += 10; // Decrease speed
        }

        if (!paused) {
            clear_window();
            
            update_grid(&grid, cyclic_rule); // Update the grid state
            draw_grid(&grid); // Draw the grid
            
            present_window();

            iterations++;
            if (iterations % save_frequency == 0) {
                write_grid_to_file(&grid, filename); // Save the current grid to file
            }
        }

        SDL_Delay(delay_ms); // Control the frame rate
    }

    // Free the grid memory before exiting
    free_grid(&grid);

    return 0;
}
