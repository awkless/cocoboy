// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "config.hpp"

#include <SDL.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>
#include <imgui.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>

#include <memory>
#include <exception>

int
main(int argc, char **argv)
try {
    std::unique_ptr<cxxopts::Options> parser = std::make_unique<cxxopts::Options>(argv[0], "- testing");
    bool version = false;
    auto& options = *parser;
    options.set_width(80).set_tab_expansion().add_options()(
        "v,version", "version info", cxxopts::value<bool>(version));
    auto result = options.parse(argc, argv);

    if (result.count("version"))
        fmt::print("{}\n", cocoboy::PROGRAM_VERSION);

    fmt::print("{} {}\n", cocoboy::PROGRAM_NAME, cocoboy::PROGRAM_VERSION);
    fmt::print("{}\n\n", cocoboy::PROGRAM_DESCRIPTION);

    auto logger = spdlog::stdout_color_mt(cocoboy::PROGRAM_NAME.data());
    logger->set_level(spdlog::level::trace);
    logger->info("This is a simple info message");
    logger->trace("This is a simple trace message");
    logger->debug("This is a simple debug message");
    logger->warn("This is a simple warning message");
    logger->error("This is a simple error message");
    logger->critical("This is a simple critical error message");

    constexpr int winWidth = 600;
    constexpr int winHeight = 400;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window =
        SDL_CreateWindow("cc8emu", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& gio = ImGui::GetIO();
    gio.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) == 1) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        bool some_panel = true;
        ImGui::Begin("Some panel", &some_panel);
        ImGui::Text("Hello world");
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // NOLINT
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
} catch (const spdlog::spdlog_ex& error) {
    fmt::print("{}\n", error.what());
    return 1;
} catch (const cxxopts::exceptions::exception& error) {
    fmt::print("{}\n", error.what());
    return 1;
} catch (const std::exception& error) {
    fmt::print("{}\n", error.what());
    return 1;
}
