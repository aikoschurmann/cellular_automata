/*
 * GUI.c
 *
 *  Created on: 04 Oct 2024
 *      Author: mvdcamme
 */

#include "GUI.h"

/*
 * Deze renderer wordt gebruikt om figuren in het venster te tekenen. De renderer
 * wordt geïnitialiseerd in de initialize_window-functie.
 */
static SDL_Renderer *renderer;


/*
 * Onderstaande twee lijnen maken deel uit van de minimalistische voorbeeldapplicatie:
 * ze houden de laatste positie bij waar de gebruiker geklikt heeft.
 */
int mouse_x = 0;
int mouse_y = 0;

/*
 * Geeft aan of de applicatie moet verdergaan.
 * Dit is waar zolang de gebruiker de applicatie niet wilt afsluiten door op het kruisje te klikken.
 */
int should_continue = 1;

/*
 * Dit is het venster dat getoond zal worden en waarin het speelveld weergegeven wordt.
 * Dit venster wordt aangemaakt bij het initialiseren van de GUI en wordt weer afgebroken
 * wanneer het spel ten einde komt.
 */
static SDL_Window *window;

/*
 * Controleert of het gegeven event "relevant" is voor dit spel.
 * We gebruiken in deze GUI enkel muiskliks, toetsdrukken, en de "Quit"
 * van het venster, dus enkel deze soorten events zijn "relevant".
 */
static int is_relevant_event(SDL_Event *event) {
    if (event == NULL) {
        return 0;
    }
    return (event->type == SDL_MOUSEBUTTONDOWN) ||
           (event->type == SDL_KEYDOWN) ||
           (event->type == SDL_QUIT);
}

/*
 * Vangt de input uit de GUI op. Deze functie is al deels geïmplementeerd, maar je moet die
 * zelf nog afwerken.
 * Je mag natuurlijk alles aanpassen aan deze functie, inclusies return-type en argumenten.
 */
void read_input() {
    SDL_Event event;

    /*
     * Handelt alle input uit de GUI af.
     * Telkens de speler een input in de GUI geeft (bv. een muisklik, muis bewegen, toetsindrukken
     * enz.) wordt er een 'event' (van het type SDL_Event) gegenereerd dat hier wordt afgehandeld.
     *
     * Niet al deze events zijn relevant voor jou: als de muis bv. gewoon wordt bewogen, hoef
     * je niet te reageren op dit event.
     * We gebruiken daarom de is_relevant_event-functie om niet-gebruikte events weg te
     * filteren, zonder dat ze de applicatie vertragen of de GUI minder responsief maken.
     *
     * Zie ook https://wiki.libsdl.org/SDL_PollEvent
     */
    while (1) {
        int event_polled = SDL_PollEvent(&event);
        if (event_polled == 0) {
            return;
        } else if (is_relevant_event(&event)) {
            break;
        }
    }

    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_p) {
            printf("P pressed\n");
        }
        break;
    case SDL_QUIT:
        // De gebruiker heeft op het kruisje van het venster geklikt om de applicatie te stoppen.
        should_continue = 0;
        break;
    case SDL_MOUSEBUTTONDOWN:
        /*
         * De speler heeft met de muis geklikt: met de onderstaande lijn worden de coördinaten in het
         * het speelveld waar de speler geklikt heeft bewaard in de variabelen mouse_x en mouse_y.
         */
        mouse_x = event.button.x;
        mouse_y = event.button.y;
        printf("Clicked at (%i,%i)\n", mouse_x, mouse_y);
        break;
    }
}

/*
 * Maakt het volledige scherm terug wit.
 */
void clear_window() {
    // Maakt van wit de achtergrondkleur.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    /*
     * Reset het venster en verandert het naar de kleur
     * die op de lijn hierboven werd vastgelegd.
     */
    SDL_RenderClear(renderer);
}

/*
 * Tekent een rechthoek op positie (x,y), met de gegeven breedte en hoogte.
 */
void draw_rectangle(int x, int y, int width, int height) {
    SDL_Rect rectangle = { x, y, width, height};
    /*
     * Vraagt de renderer om rectangle te tekenen, in de kleur
     * die werd vastgelegd via SDL_SetRenderDrawColor.
     */
    SDL_RenderFillRect(renderer, &rectangle);
}

void draw_window() {
    clear_window();

    /*
     * Tekent de eerste figuur in het rood.
     * Zie https://wiki.libsdl.org/SDL_SetRenderDrawColor
     */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_rectangle(mouse_x, mouse_y, 30, 30);

    // Tekent de tweede figuur in het groen.
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    draw_rectangle(100, 100, 20, 20);

    // Tekent de derde figuur in het blauw.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    draw_rectangle(200, 200, 50, 50);

    /*
     * De volgende lijn moet zeker uitgevoerd worden op het einde van de functie.
     * Wanneer aan de renderer gevraagd wordt om iets te tekenen, wordt het venster pas aangepast
     * wanneer de SDL_RenderPresent-functie wordt aangeroepen.
     */
    SDL_RenderPresent(renderer);
}

/*
 * Initialiseert het venster en alle extra structuren die nodig zijn om het venster te manipuleren.
 * Code o.a. gebaseerd op:
 * http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
 */
void initialize_window(const char *title, int window_width, int window_height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Maak het venster aan met de gegeven dimensies en de gegeven titel. */
    window = SDL_CreateWindow(title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        // Er ging iets verkeerd bij het initialiseren.
        printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialiseert de renderer.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    // Laat de default-kleur die de renderer in het venster tekent wit zijn.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

/*
 * Dealloceert alle SDL structuren die geïnitialiseerd werden.
 */
void free_gui() {
    // Dealloceert de renderer.
    SDL_DestroyRenderer(renderer);
    // Dealloceert het venster.
    SDL_DestroyWindow(window);

    // Sluit SDL af.
    SDL_Quit();
}

/*
 * Initialiseert het venster waarin het speelveld getoond zal worden.
 * Deze functie moet aangeroepen worden aan het begin van het spel,
 * vooraleer je het speelveld begint te tekenen.
 */
void initialize_gui(int window_width, int window_height) {
    initialize_window("Automaton", window_width, window_height);
}



int main(int argc, char *argv[]) {
    initialize_gui(WINDOW_WIDTH,WINDOW_HEIGHT);
    while (should_continue) {
        draw_window();
        read_input();
    }
    // Dealloceer al het geheugen dat werd aangemaakt door SDL zelf.
    free_gui();
    return 0;
}
