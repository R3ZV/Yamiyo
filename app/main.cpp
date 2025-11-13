#define SDL_MAIN_USE_CALLBACKS 1


#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>

#include "../src/game.h"

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_sdl3.h"
#include "../libs/imgui/imgui_impl_sdlrenderer3.h"

struct AppState {
public:
    static const char* GAME_NAME;
    static const char* GAME_VERSION;

    static const int32_t WIN_WIDTH = 1280;
    static const int32_t WIN_HEIGHT = 1024;
    static const int32_t MAX_ENTITIES = 100'000;

    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO* imgui_io;
    GameState game_state;

    Uint64 last_step;
    double fps;

    bool debug_console;
    int32_t entities_cnt;
    int32_t last_entities_cnt;

    AppState(SDL_Window* win, SDL_Renderer* renderer) :
        window(win),
        renderer(renderer),
        imgui_io(nullptr),
        game_state(0, renderer, WIN_WIDTH, WIN_HEIGHT),
        debug_console(false),
        last_step(SDL_GetPerformanceCounter())
    {}
};

const char* AppState::GAME_NAME = "Yamiyo";
const char* AppState::GAME_VERSION = "v0.1.0";

SDL_AppResult
SDL_AppInit(void** appstate, int argc, char** argv) {
    SDL_SetAppMetadata(AppState::GAME_NAME, AppState::GAME_VERSION, "");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow(
        AppState::GAME_NAME,
        AppState::WIN_WIDTH * main_scale,
        AppState::WIN_HEIGHT * main_scale,
        window_flags
    );

    if (window == nullptr) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    AppState* state = new AppState(window, renderer);
    if (!state) {
        return SDL_APP_FAILURE;
    }


    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->debug_console = false;
    state->imgui_io = &io;
    state->game_state.randomise_entities();

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState* state = (AppState*) appstate;
    ImGui_ImplSDL3_ProcessEvent(event);
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
        event->window.windowID == SDL_GetWindowID(state->window)) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key== SDLK_D && event->key.mod == SDL_KMOD_LSHIFT) {
        state->debug_console = !state->debug_console;
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}


void
compute_fps(void* appstate) {
    AppState* state = (AppState*) appstate;

    Uint64 curr_step = SDL_GetPerformanceCounter();
    Uint64 ticks = SDL_abs(state->last_step - curr_step);

    double seconds = (double)ticks / SDL_GetPerformanceFrequency();
    if (seconds > 0.0) {
        state->fps = 1.0 / seconds;
    }

    state->last_step = curr_step;
}

SDL_AppResult
SDL_AppIterate(void* appstate) {
    AppState* state = (AppState*) appstate;

    if (SDL_GetWindowFlags(state->window) & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(10);
        return SDL_APP_CONTINUE;
    }

    compute_fps(appstate);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (state->debug_console) {
        ImGui::Begin("Debug Console");
        ImGui::Text("FPS: %f", state->fps);
        ImGui::Text("Memory usage: TODO");

        ImGui::SliderInt("Entities", &state->entities_cnt, 0, state->MAX_ENTITIES);

        ImGui::End();
    }

    ImGui::Render();
    SDL_SetRenderScale(state->renderer, state->imgui_io->DisplayFramebufferScale.x, state->imgui_io->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(state->renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    SDL_RenderClear(state->renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), state->renderer);

    if (state->last_entities_cnt != state->entities_cnt) {
        state->game_state.entities_cnt = state->entities_cnt;
        state->last_entities_cnt = state->entities_cnt;
        state->game_state.randomise_entities();
    }

    state->game_state.render_entities();
    SDL_RenderPresent(state->renderer);
    state->game_state.update_entities();
    state->game_state.check_collisions(state->WIN_WIDTH, state->WIN_HEIGHT);

    return SDL_APP_CONTINUE;
}

void
SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *state = (AppState*)appstate;
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
        SDL_Quit();
    }
}
