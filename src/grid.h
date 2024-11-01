#ifndef GRID_H
#define GRID_H

#include "gui.h" 


#define CELL_SIZE 2         

struct cell {
    int state; 
};

struct grid {
    struct cell **grid1; 
    struct cell **grid2; 
    int current;        
    int width;        
    int height;         
    struct color *palette; 
    int states; 
};

struct color {
    int red;  
    int green; 
    int blue;  
};

struct cell** mallocgrid(int width, int height);
struct color* mallocpalette(int total_states);
void free_palette(struct color *palette);
void free_grid(struct grid *grid);

void initialize_black_and_white_palette(struct color *palette);
void initialize_gradient_palette(struct color *palette, struct color *start, struct color *end, int total_states);
void initialize_random_palette(struct color *palette, int total_states);
void initialize_random_grid(struct cell **grid, int width, int height, int states);
void initialize_grid(struct grid *grid, int width, int height, int states, struct color* palette);

int count_live_neighbors(int x, int y, struct cell **grid, struct grid *grid_info);
int has_successor(int x, int y, struct cell **grid, struct grid *grid_info);

void update_grid(struct grid *grid, int (*rule_function)(int, int, struct cell **, struct grid *));

int conways_game_of_life_rule(int x, int y, struct cell **grid, struct grid *grid_info);
int highlife_rule(int x, int y, struct cell **grid, struct grid *grid_info);
int cyclic_rule(int x, int y, struct cell **grid, struct grid *grid_info);

void draw_cell(int x, int y, struct cell *cell, struct color *palette);
void draw_grid(struct grid *grid);
void write_grid_to_file(struct grid *grid, const char* filename);
#endif 
