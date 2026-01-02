#include "entity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static SDL_Texture* wall_texture = NULL;
typedef struct {
    float x, y;
} WallPos;
static WallPos walls[MAX_entities];
static int wall_count = 0;
static void render(SDL_Renderer* renderer) {
    for (int i = 0; i < wall_count; i++) {
        SDL_FRect rect = {walls[i].x, walls[i].y, 50.0f, 50.0f};
        SDL_RenderTexture(renderer, wall_texture, NULL, &rect);
    }
}
// These empty functions prevent the "Segmentation Fault" crash
static void wall_quit(void) {}   
static void wall_handle_events(SDL_Event* event) { (void)event; }
static void wall_update(void) {}

Entity init_wall(SDL_Renderer* renderer, float x, float y) {
    if (!wall_texture) {
        wall_texture = IMG_LoadTexture(renderer, "wall.png");
    }
    if (wall_count < MAX_entities) {
        walls[wall_count++] = (WallPos){x, y};
    }

    return (Entity){
        .quit = wall_quit,
        .handle_events = wall_handle_events,
        .update = wall_update,
        .render = render
    };
}