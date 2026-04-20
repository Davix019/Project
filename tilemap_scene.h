#ifndef TILEMAP_SCENE_H
#define TILEMAP_SCENE_H

#include "sfw.h"
#include "tilemap.h"
#include "player.h"
#include "enemy.h"

enum GameState { STATE_MENU, STATE_PLAY, STATE_EDITOR, STATE_GAMEOVER, STATE_WIN };

class TileMapScene : public Scene {
    SFW_OBJECT(TileMapScene, Scene);

public:
    TileMap* myMap;
    Player* myPlayer;
    EnemyBase* enemies[4];

    GameState currentState;
    int currentTool;
    int keysCollected;
    int keysNeeded;
    int currentLevel;

    TileMapScene() {
        myMap = new TileMap(20, 15);
        myPlayer = new Player(64, 64);
        enemies[0] = new Scout();
        enemies[1] = new Scout();
        enemies[2] = new Tank();
        enemies[3] = new Tank();

        currentState = STATE_MENU;
        currentTool = 1;
        keysCollected = 0;
        keysNeeded = 0;
        currentLevel = 0;
    }

    ~TileMapScene() {
        for(int i=0; i<4; ++i) delete enemies[i];
        delete myPlayer;
        delete myMap;
    }

    void clearMap() {
        for (int y = 0; y < 15; ++y) {
            for (int x = 0; x < 20; ++x) {
                myMap->setTile(x, y, 1);
            }
        }
    }

    void paint(Vector2 pos) {
        if (currentState != STATE_EDITOR) return;
        int tileX = (int)(pos.x / 32.0f);
        int tileY = (int)(pos.y / 32.0f);
        myMap->setTile(tileX, tileY, currentTool);
    }

    void setupCampaignLevel(int lvl) {
        currentLevel = lvl;
        keysCollected = 0;
        clearMap();
        for(int i=0; i<4; ++i) enemies[i]->active = false;

        switch(lvl) {
            case 1:
                keysNeeded = 1;
                for(int x=4; x<16; ++x) { myMap->setTile(x, 5, 2); myMap->setTile(x, 10, 2); }
                myMap->setTile(10, 8, 3);
                myMap->setTile(18, 13, 4);
                myPlayer->reset(64, 240);
                enemies[0]->reset(550, 240, true);
                break;
            case 2:
                keysNeeded = 2;
                for(int y=0; y<15; ++y) { if(y != 7 && y != 8) myMap->setTile(10, y, 2); }
                myMap->setTile(2, 2, 3);
                myMap->setTile(2, 12, 3);
                myMap->setTile(18, 7, 4);
                myPlayer->reset(550, 240);
                enemies[0]->reset(64, 64, true);
                enemies[2]->reset(64, 400, true);
                break;
            case 3:
                keysNeeded = 3;
                for(int x=4; x<17; x+=4) for(int y=3; y<13; y+=4) {
                    myMap->setTile(x, y, 2); myMap->setTile(x+1, y, 2);
                    myMap->setTile(x, y+1, 2); myMap->setTile(x+1, y+1, 2);
                }
                myMap->setTile(1, 1, 3); myMap->setTile(18, 1, 3); myMap->setTile(10, 13, 3);
                myMap->setTile(1, 13, 4);
                myPlayer->reset(320, 240);
                enemies[0]->reset(50, 50, true);
                enemies[1]->reset(580, 50, true);
                enemies[2]->reset(580, 420, true);
                break;
            case 4:
                keysNeeded = 3;
                for(int x=0; x<16; ++x) myMap->setTile(x, 4, 2);
                for(int x=4; x<20; ++x) myMap->setTile(x, 9, 2);
                myMap->setTile(18, 2, 3); myMap->setTile(2, 6, 3); myMap->setTile(18, 12, 3);
                myMap->setTile(2, 13, 4);
                myPlayer->reset(64, 32);
                enemies[0]->reset(500, 200, true);
                enemies[2]->reset(100, 300, true);
                enemies[3]->reset(500, 400, true);
                break;
            case 5:
                keysNeeded = 4;
                for(int i=0; i<20; ++i) { myMap->setTile(i, 0, 2); myMap->setTile(i, 14, 2); }
                for(int i=0; i<15; ++i) { myMap->setTile(0, i, 2); myMap->setTile(19, i, 2); }
                for(int i=5; i<15; ++i) { myMap->setTile(i, 5, 2); myMap->setTile(i, 9, 2); }
                myMap->setTile(2, 2, 3); myMap->setTile(17, 2, 3);
                myMap->setTile(2, 12, 3); myMap->setTile(17, 12, 3);
                myMap->setTile(10, 7, 4);
                myPlayer->reset(320, 40);
                for(int i=0; i<4; ++i) enemies[i]->reset(320 + (i*40), 400, true);
                break;
        }
        currentState = STATE_PLAY;
    }

    virtual void render() {
        Renderer::get_singleton()->camera_2d_projection_set_to_window();
        float dt = ImGui::GetIO().DeltaTime;

        if (currentState == STATE_PLAY) {
            myPlayer->update(dt, myMap);
            for(int i=0; i<4; ++i) {
                if(enemies[i]->active) {
                    enemies[i]->update(dt, myMap, myPlayer);
                    if (myPlayer->position.distance_to(enemies[i]->position) < enemies[i]->size) currentState = STATE_GAMEOVER;
                }
            }
            int tx = (int)(myPlayer->position.x / 32.0f), ty = (int)(myPlayer->position.y / 32.0f);
            int currentTile = myMap->getTile(tx, ty);
            if (currentTile == 3) { myMap->setTile(tx, ty, 1); keysCollected++; }
            if (currentTile == 4 && keysCollected >= keysNeeded) {
                if (currentLevel < 5) setupCampaignLevel(currentLevel + 1);
                else currentState = STATE_WIN;
            }
        }

        if (currentState == STATE_PLAY || currentState == STATE_EDITOR) {
            myMap->render();
            if (currentState == STATE_PLAY) {
                myPlayer->render();
                for(int i=0; i<4; ++i) enemies[i]->render();
            }
        }
        Renderer::get_singleton()->camera_2d_reset();

        ImGui::Begin("Game Menu");
        if (currentState == STATE_MENU) {
            ImGui::Text("--- CAMPAIGN ---");
            if (ImGui::Button("Start Mission (Lvl 1)")) setupCampaignLevel(1);
            ImGui::Separator();
            ImGui::Text("--- CUSTOM ---");
            if (ImGui::Button("Play Editor Map")) {
                currentLevel = 0; keysCollected = 0; keysNeeded = 0;
                myMap->load_map("level_data.dat");
                myPlayer->reset(100, 100);
                enemies[0]->reset(400, 400, true);
                enemies[2]->reset(500, 500, true);
                currentState = STATE_PLAY;
            }
            if (ImGui::Button("Level Editor")) { myMap->load_map("level_data.dat"); currentState = STATE_EDITOR; }
        } else if (currentState == STATE_EDITOR) {
            if (ImGui::Button("Back to Menu")) currentState = STATE_MENU;
            ImGui::Separator();
            if (ImGui::Button("Grass (1)")) currentTool = 1;
            if (ImGui::Button("Wall (2)")) currentTool = 2;
            if (ImGui::Button("Key (3)")) currentTool = 3;
            if (ImGui::Button("Exit (4)")) currentTool = 4;
            if (ImGui::Button("Erase (0)")) currentTool = 0;
            ImGui::Separator();
            if (ImGui::Button("Save Map")) myMap->save_map("level_data.dat");
            if (ImGui::Button("Load Map")) myMap->load_map("level_data.dat");
        } else if (currentState == STATE_PLAY) {
            ImGui::Text(currentLevel > 0 ? "Level: %d" : "Custom Map", currentLevel);
            ImGui::Text("Relics: %d / %d", keysCollected, keysNeeded);
            if (keysCollected >= keysNeeded && currentLevel > 0) ImGui::TextColored(ImVec4(0,1,0,1), "PORTAL ACTIVE!");
            ImGui::Separator();
            if (myPlayer->dashActiveTimer > 0) ImGui::Text("DASHING...");
            else if (myPlayer->cooldownTimer > 0) ImGui::Text("CD: %.1fs", myPlayer->cooldownTimer);
            else ImGui::Text("READY (SPACE)");
        } else if (currentState == STATE_GAMEOVER) {
            ImGui::Text("WASTED!");
            if (ImGui::Button("Retry")) { if(currentLevel == 0) { myMap->load_map("level_data.dat"); currentState = STATE_PLAY; } else setupCampaignLevel(currentLevel); }
            if (ImGui::Button("Main Menu")) currentState = STATE_MENU;
        } else if (currentState == STATE_WIN) {
            ImGui::Text("VICTORY!");
            ImGui::Text("All relics secured.");
            if (ImGui::Button("Main Menu")) currentState = STATE_MENU;
        }
        ImGui::End();
    }
};

#endif
