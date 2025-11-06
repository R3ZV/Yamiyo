#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_sdl3.h"
#include "../libs/imgui/imgui_impl_sdlrenderer3.h"

struct AppState {
    static const char* GAME_NAME;
    static const char* GAME_VERSION;

    const int32_t WIN_WIDTH = 1280;
    const int32_t WIN_HEIGHT = 1024;

    SDL_Window* window;
    SDL_Renderer* renderer;
    ImGuiIO* imgui_io;

    Uint64 last_step;
    bool debug_console;
};

const char* AppState::GAME_NAME = "Yamiyo";
const char* AppState::GAME_VERSION = "v0.1.0";

SDL_AppResult
SDL_AppInit(void** appstate, int argc, char** argv) {
    AppState* state = new AppState();
    if (!state) {
        return SDL_APP_FAILURE;
    }

    SDL_SetAppMetadata(state->GAME_NAME, state->GAME_VERSION, "");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow(state->GAME_NAME, state->WIN_WIDTH * main_scale, state->WIN_HEIGHT * main_scale, window_flags);
    if (window == nullptr) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->window = window;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->renderer = renderer;
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

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult
SDL_AppIterate(void* appstate) {
    AppState* state = (AppState*) appstate;

    if (SDL_GetWindowFlags(state->window) & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(10);
        return SDL_APP_CONTINUE;
    }

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    SDL_SetRenderScale(state->renderer, state->imgui_io->DisplayFramebufferScale.x, state->imgui_io->DisplayFramebufferScale.y);
    SDL_SetRenderDrawColorFloat(state->renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    SDL_RenderClear(state->renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), state->renderer);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
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
