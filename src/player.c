#include "player.h"
#include "crate.h"

static SDL_Texture* player_texture;
typedef struct { float x; float y; } Position;

// Spawning at center (375, 275)
Position position = {375.0f, 275.0f};

static void quit(){}

static void handle_events(SDL_Event *event){
    if (event->type == SDL_EVENT_KEY_DOWN) {
        float tileSize = 50.0f;
        float dx = 0, dy = 0;

        if (event->key.scancode == SDL_SCANCODE_LEFT) dx = -tileSize;
        else if (event->key.scancode == SDL_SCANCODE_RIGHT) dx = tileSize;
        else if (event->key.scancode == SDL_SCANCODE_UP) dy = -tileSize;
        else if (event->key.scancode == SDL_SCANCODE_DOWN) dy = tileSize;

        if (dx != 0 || dy != 0) {
            float next_x = position.x + dx;
            float next_y = position.y + dy;

            // 1. WINDOW BORDER CHECK
            if (next_x < 0 || next_x >= 800 || next_y < 0 || next_y >= 600) return;

            // 2. CRATE PUSH CHECK
            // If try_push_crate returns true, it means either:
            // a) No crate was there
            // b) A crate was there and it was successfully pushed
            if (try_push_crate(next_x, next_y, dx, dy)) {
                position.x = next_x;
                position.y = next_y;
            }
        }
    }
}

static void update(){}

static void render(SDL_Renderer* renderer){
   SDL_FRect player_rect = {position.x, position.y, 50.0f, 50.0f};
   SDL_RenderTexture(renderer, player_texture, NULL, &player_rect);
}

Entity init_player(SDL_Renderer* renderer){
   player_texture = IMG_LoadTexture(renderer, "./imageplayerfacingdownneutral.png");
   return (Entity){.quit=quit, .handle_events=handle_events, .update=update, .render=render};    


   Entity player={.quit=quit,
                   .handle_events=handle_events,
                   .update=update,
                   .render=render};    
   return player;
}