#include "tilemap_application.h"

TileMapApplication::TileMapApplication() {
    Renderer::initialize();
    GUI::initialize();
}

TileMapApplication::~TileMapApplication() {
}

void TileMapApplication::render() {
    Renderer::get_singleton()->clear_screen(Color(0.2, 0.2, 0.2, 1.0));

    GUI::new_frame();

    Application::render();

    ImGui::Begin("TileMap Szerkeszto - Sprint 1");
    ImGui::Text("Sikeresen mukodik az SFW motor!");
    ImGui::End();

    GUI::render();
}
