#ifndef TILEMAP_SCENE_H
#define TILEMAP_SCENE_H

#include "sfw.h"
#include "tilemap.h"

class TileMapScene : public Scene {
    SFW_OBJECT(TileMapScene, Scene);

public:
    TileMap* myMap;
    int currentTool;
    int lastClickedX;
    int lastClickedY;

    TileMapScene() {
        myMap = new TileMap(4, 4);
        currentTool = 1;
        lastClickedX = -1;
        lastClickedY = -1;
    }

    ~TileMapScene() {
        delete myMap;
    }

    void paint(Vector2 pos) {
        int tileX = (int)(pos.x / 32.0f);
        int tileY = (int)(pos.y / 32.0f);

        lastClickedX = tileX;
        lastClickedY = tileY;

        myMap->setTile(tileX, tileY, currentTool);
    }

    virtual void render() {
        Renderer::get_singleton()->camera_2d_projection_set_to_window();

        myMap->render();

        Renderer::get_singleton()->camera_2d_reset();

        ImGui::Begin("TileMap Editor");
        ImGui::Text("Draw on the map with the left mouse button!");
        ImGui::Separator();
        ImGui::Text("Last click: X: %d, Y: %d", lastClickedX, lastClickedY);
        ImGui::Separator();
        ImGui::Text("Selected tool:");
        if (ImGui::Button("Grass (Green) - 1")) currentTool = 1;
        if (ImGui::Button("Wall (Gray) - 2")) currentTool = 2;
        if (ImGui::Button("Erase (Empty) - 0")) currentTool = 0;

        ImGui::Separator();
        if (ImGui::Button("Save Map")) {
            myMap->save_map("level_data.dat");
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Map")) {
            myMap->load_map("level_data.dat");
        }
        ImGui::End();
    }
};

#endif
