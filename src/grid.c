#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Function to allocate memory for a 2D grid of cells
struct cell** mallocgrid(int width, int height) {
    struct cell** grid = malloc(width * sizeof(struct cell *));
    if (!grid) {
        fprintf(stderr, "Memory allocation failed for grid!\n");
        exit(1);
    }
    for (int i = 0; i < width; i++) {
        grid[i] = malloc(height * sizeof(struct cell));
        if (!grid[i]) {
            fprintf(stderr, "Memory allocation failed for grid row %d!\n", i);
            exit(1);
        }
    }
    return grid;
}

// Function to allocate memory for a color palette
struct color* mallocpalette(int total_states) {
    struct color* palette = malloc(total_states * sizeof(struct color));
    if (!palette) {
        fprintf(stderr, "Memory allocation failed for palette!\n");
        exit(1);
    }
    return palette;
}

// Free allocated color palette memory
void free_palette(struct color *palette) {
    free(palette);
}

// Free allocated grid memory
void free_grid(struct grid *grid) {
    for (int i = 0; i < grid->width; i++) {
        free(grid->grid1[i]);
        free(grid->grid2[i]);
    }
    free(grid->grid1);
    free(grid->grid2);
    free(grid->palette);
}

// Initialize a black-and-white palette
void initialize_black_and_white_palette(struct color *palette) {
    palette[0] = (struct color){0, 0, 0};     // Black
    palette[1] = (struct color){255, 255, 255}; // White
}

// Initialize a gradient palette between two colors
void initialize_gradient_palette(struct color *palette, struct color *start, struct color *end, int total_states) {
    for (int i = 0; i < total_states; i++) {
        float t = (float)i / (total_states - 1); // Normalize index to [0, 1]
        palette[i].red = (int)(start->red + t * (end->red - start->red));
        palette[i].green = (int)(start->green + t * (end->green - start->green));
        palette[i].blue = (int)(start->blue + t * (end->blue - start->blue));

        // Clamp color values to the range [0, 255]
        palette[i].red = (palette[i].red < 0) ? 0 : (palette[i].red > 255) ? 255 : palette[i].red;
        palette[i].green = (palette[i].green < 0) ? 0 : (palette[i].green > 255) ? 255 : palette[i].green;
        palette[i].blue = (palette[i].blue < 0) ? 0 : (palette[i].blue > 255) ? 255 : palette[i].blue;
    }
}

// Initialize a random palette
void initialize_random_palette(struct color *palette, int total_states) {
    for (int i = 0; i < total_states; i++) {
        palette[i].red = rand() % 256;
        palette[i].green = rand() % 256;
        palette[i].blue = rand() % 256;
    }
}

// Initialize a random grid with given states
void initialize_random_grid(struct cell **grid, int width, int height, int states) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            grid[i][j].state = rand() % states;
        }
    }
}

// Initialize the grid structure
void initialize_grid(struct grid *grid, int width, int height, int states, struct color* palette) {
    grid->width = width;
    grid->height = height;
    grid->current = 0; // Start with grid1

    grid->grid1 = mallocgrid(width, height);
    grid->grid2 = mallocgrid(width, height);
    grid->states = states;
    grid->palette = palette;

    // Initialize grid1 with random states
    initialize_random_grid(grid->grid1, width, height, states);
}



// Count live neighbors of a cell
int count_live_neighbors(int x, int y, struct cell **grid, struct grid *grid_info) {
    int live_neighbors = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if ((dx || dy) && x + dx >= 0 && x + dx < grid_info->width && y + dy >= 0 && y + dy < grid_info->height) {
                live_neighbors += grid[x + dx][y + dy].state;
            }
        }
    }
    return live_neighbors;
}

// Check if a cell has a successor in the next generation
int has_successor(int x, int y, struct cell **grid, struct grid *grid_info) {
    int target_state = (grid[x][y].state + 1) % grid_info->states;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if ((dx || dy) && x + dx >= 0 && x + dx < grid_info->width && y + dy >= 0 && y + dy < grid_info->height) {
                if (grid[x + dx][y + dy].state == target_state) return 1;
            }
        }
    }
    return 0;
}

// Update the grid based on the specified rule function
void update_grid(struct grid *grid, int (*rule_function)(int, int, struct cell **, struct grid *)) {
    struct cell **grid_current = (grid->current == 0) ? grid->grid1 : grid->grid2;
    struct cell **grid_next = (grid->current == 0) ? grid->grid2 : grid->grid1;

    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            grid_next[x][y].state = rule_function(x, y, grid_current, grid);
        }
    }

    grid->current = 1 - grid->current; // Toggle between 0 and 1
}

// Rules for the cellular automaton
int conways_game_of_life_rule(int x, int y, struct cell **grid, struct grid *grid_info) {
    int live_neighbors = count_live_neighbors(x, y, grid, grid_info);
    return (grid[x][y].state == 1) ? (live_neighbors == 2 || live_neighbors == 3) : (live_neighbors == 3);
}

int highlife_rule(int x, int y, struct cell **grid, struct grid *grid_info) {
    int live_neighbors = count_live_neighbors(x, y, grid, grid_info);
    return (grid[x][y].state == 1) ? (live_neighbors == 1 || live_neighbors == 3 || live_neighbors == 5) : (live_neighbors == 3);
}

int cyclic_rule(int x, int y, struct cell **grid, struct grid *grid_info) {
    return has_successor(x, y, grid, grid_info) ? (grid[x][y].state + 1) % grid_info->states : grid[x][y].state;
}

// Draw a single cell using its color from the palette
void draw_cell(int x, int y, struct cell *cell, struct color *palette) {
    struct color col = palette[cell->state];
    SDL_SetRenderDrawColor(renderer, col.red, col.green, col.blue, 255);
    draw_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
}

// Draw the entire grid
void draw_grid(struct grid *grid) {
    struct cell **grid_current = (grid->current == 0) ? grid->grid1 : grid->grid2;
    for (int i = 0; i < grid->width; i++) {
        for (int j = 0; j < grid->height; j++) {
            draw_cell(i, j, &grid_current[i][j], grid->palette);
        }
    }
}

// Write the grid's state to a file
void write_grid_to_file(struct grid *grid, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file!\n");
        exit(1);
    }

    fprintf(file, "%d %d\n", grid->width, grid->height);
    struct cell **grid_current = (grid->current == 0) ? grid->grid1 : grid->grid2;
    for (int i = 0; i < grid->width; i++) {
        for (int j = 0; j < grid->height; j++) {
            fprintf(file, "%d ", grid_current[i][j].state);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
