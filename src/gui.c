/*
    Heavily inspired from the GUI.c file provided on canvas
*/

#include "gui.h"


SDL_Renderer *renderer;
static SDL_Window *window;

static const Uint8 *keyboard_state = NULL; // Current keyboard state
static Uint8 previous_keyboard_state[SDL_NUM_SCANCODES]; // Previous keyboard state

int mouse_location[2] = {0, 0};
int mouse_clicked = 0;
int should_continue = 1;

// Initialize keyboard state (call this function once at the start of the program)
void initialize_keyboard_state() {
    keyboard_state = SDL_GetKeyboardState(NULL);
    memcpy(previous_keyboard_state, keyboard_state, sizeof(previous_keyboard_state));
}

// Update keyboard state (call this function once per frame)
void update_keyboard_state() {
    memcpy(previous_keyboard_state, keyboard_state, sizeof(previous_keyboard_state));
    keyboard_state = SDL_GetKeyboardState(NULL);
}

// Check if a specific key is pressed
int is_key_pressed(SDL_Scancode key) {
    return keyboard_state && keyboard_state[key];
}

// Check if a specific key is pressed just now
int is_key_down(SDL_Scancode key) {
    return keyboard_state && keyboard_state[key] && !previous_keyboard_state[key];
}

// Check if a specific key is released just now
int is_key_up(SDL_Scancode key) {
    return keyboard_state && !keyboard_state[key] && previous_keyboard_state[key];
}

// Process all SDL events
void handle_events() {
    SDL_Event event;

    // Update keyboard state before processing events
    update_keyboard_state();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                should_continue = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_location[0] = event.button.x;
                mouse_location[1] = event.button.y;
                mouse_clicked = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_clicked = 0;
                break;
            default:
                break;
        }
    }
}

// Clear the window to white
void clear_window() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

// Present the drawn window
void draw_window() {
    clear_window();
    SDL_RenderPresent(renderer);
}

// Initialize the SDL window and renderer
void initialize_window(const char *title, int window_width, int window_height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}



// Cleanup resources before exiting
void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_rectangle(int x, int y, int width, int height) {
    SDL_Rect rectangle = { x, y, width, height};
    SDL_RenderFillRect(renderer, &rectangle);
}

void present_window(){
    SDL_RenderPresent(renderer);
}

void set_color(int r, int g, int b, int a){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}