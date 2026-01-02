#include "entity.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static SDL_Texture* crate_texture = NULL;

typedef struct {
    float x, y;
} CratePos;

static CratePos crates[MAX_entities];
static int crate_count = 0;

bool try_push_crate(float target_x, float target_y, float dx, float dy) {
    for (int i = 0; i < crate_count; i++) {
        if (crates[i].x == target_x && crates[i].y == target_y) {
            float next_x = crates[i].x + dx;
            float next_y = crates[i].y + dy;

            if (next_x < 0 || next_x >= 800 || next_y < 0 || next_y >= 600) return false;

            for (int j = 0; j < crate_count; j++) {
                if (crates[j].x == next_x && crates[j].y == next_y) return false;
            }

            crates[i].x = next_x;
            crates[i].y = next_y;
            return true;
        }
    }
    return true; 
}

static void render(SDL_Renderer* renderer) {
    for (int i = 0; i < crate_count; i++) {
        SDL_FRect rect = {crates[i].x, crates[i].y, 50.0f, 50.0f};
        SDL_RenderTexture(renderer, crate_texture, NULL, &rect);
    }
}

// These empty functions prevent the "Segmentation Fault" crash
static void crate_quit(void) {}
static void crate_handle_events(SDL_Event* event) { (void)event; }
static void crate_update(void) {}

Entity init_crate(SDL_Renderer* renderer, float x, float y) {
    if (!crate_texture) {
        crate_texture = IMG_LoadTexture(renderer, "woodcrate.png");
    }
    if (crate_count < MAX_entities) {
        crates[crate_count++] = (CratePos){x, y};
    }

    return (Entity){
        .quit = crate_quit,
        .handle_events = crate_handle_events,
        .update = crate_update,
        .render = render
    };
}