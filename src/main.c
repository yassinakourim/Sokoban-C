#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "entity.h"
#include "player.h"
#include "crate.h"


#define HANDLE_EVENTS_ENTITIES(entities, entities_count, event) \
    for (int i = 0; i < entities_count; i++) { \
        entities[i].handle_events(event); \
    }

#define QUIT_ENTITIES(entities, entities_count, renderer) \
    for (int i = 0; i < entities_count; i++) { \
        entities[i].quit(); \
    }

#define UPDATE_ENTITIES(entities, entities_count, renderer) \
    for (int i = 0; i < entities_count; i++) { \
        entities[i].update(); \
    }

#define RENDER_ENTITIES(entities, entities_count, renderer) \
    for (int i = 0; i < entities_count; i++) { \
        entities[i].render(renderer); \
    }

SDL_Window* window;
SDL_Renderer* renderer;


Entity entities[MAX_entities];
int entities_count = 0;

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    (void)appstate;  // Unused for now
    (void)result;    // Unused for now
    QUIT_ENTITIES(entities, entities_count, renderer);
    SDL_DestroyRenderer(renderer);
    renderer = NULL;                
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    (void)appstate;  // Unused for now
    HANDLE_EVENTS_ENTITIES(entities, entities_count, event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  // Exit the app
    }

    return SDL_APP_CONTINUE;
}

void update() {
    UPDATE_ENTITIES(entities, entities_count, renderer);
}

void render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);

    RENDER_ENTITIES(entities, entities_count, renderer);

    SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    (void)appstate;  // Unused for now
    
    update(); 
    render();
    return SDL_APP_CONTINUE;
   
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    (void)appstate;  // Unused for now
    (void)argc;      // Unused
    (void)argv;      // Unused
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error initializing SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    // SDL3: CreateWindow takes (title, width, height, flags)
    // Pass 0 for flags for a normal window
    window = SDL_CreateWindow("Sokoban othmane", 800, 600, 0);
    if (!window) {
        SDL_Log("Error creating window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Only add one player and one crate
    entities[entities_count++] = init_player(renderer);
    entities[entities_count++] = init_crate(renderer, 525.0f, 275.0f);
    entities[entities_count++] = init_crate(renderer, 625.0f, 275.0f);

    return SDL_APP_CONTINUE;
}