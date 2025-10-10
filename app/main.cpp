#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct Player {
    SDL_FRect rect;

    static Player init() {
        return Player {
            .rect = SDL_FRect {
                .x = 250,
                .y = 150,
                .w = 100,
                .h = 200,
            },
        };
    }
};

struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player;
    Uint64 last_step;
};

SDL_AppResult
SDL_AppInit(void** appstate, int argc, char** argv) {
    SDL_SetAppMetadata("Example HUMAN READABLE NAME", "1.0", "com.example.CATEGORY-NAME");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState* state = (AppState*)SDL_calloc(1, sizeof(AppState));
    if (!state) {
        return SDL_APP_FAILURE;
    }

    *appstate = state;
    state->player = Player::init();

    if (!SDL_CreateWindowAndRenderer("Yamiyo", 640, 480, SDL_WINDOW_RESIZABLE, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(state->renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate(void* appstate) {
    AppState* state = (AppState*) appstate;
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(state->renderer, &state->player.rect);

    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *state = (AppState*)appstate;
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
}
