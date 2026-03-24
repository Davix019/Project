#include "tilemap_application.h"

TileMapApplication::TileMapApplication() {
    Renderer::initialize();
    GUI::initialize();

    myMap = new TileMap(4, 4);
}

TileMapApplication::~TileMapApplication() {
    delete myMap;
}

void TileMapApplication::render() {
    Renderer::get_singleton()->clear_screen(Color(0.2, 0.2, 0.2, 1.0));

    myMap->render();

    GUI::new_frame();
    Application::render();

    ImGui::Begin("TileMap Szerkeszto");
    ImGui::Text("A palya lathato!");
    ImGui::End();

    GUI::render();
}
