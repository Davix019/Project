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

        ImGui::Begin("TileMap Szerkeszto");
        ImGui::Text("Rajzolj a bal egergombbal a palyara!");
        ImGui::Separator();
        ImGui::Text("Utolso kattintas: X: %d, Y: %d", lastClickedX, lastClickedY);
        ImGui::Separator();
        ImGui::Text("Kivalasztott eszkoz:");
        if (ImGui::Button("Fu (Zold) - 1")) currentTool = 1;
        if (ImGui::Button("Fal (Szurke) - 2")) currentTool = 2;
        if (ImGui::Button("Torles (Ures) - 0")) currentTool = 0;
        ImGui::End();
    }
};

#endif
