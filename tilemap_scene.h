#ifndef TILEMAP_SCENE_H
#define TILEMAP_SCENE_H

#include "sfw.h"
#include "tilemap.h"
#include "player.h"
#include "enemy.h"

enum GameState {
    STATE_MENU,
    STATE_PLAY,
    STATE_EDITOR,
    STATE_GAMEOVER,
    STATE_WIN
};

class TileMapScene : public Scene {
    SFW_OBJECT(TileMapScene, Scene);

public:
    TileMap* myMap;
    Player* myPlayer;
    Enemy* myEnemy;
    GameState currentState;
    int currentTool;

    int keysCollected;
    int keysNeeded;
    int currentLevel; // 0: Custom, 1: Level 1, 2: Level 2...

    TileMapScene() {
        myMap = new TileMap(20, 15);
        myPlayer = new Player(64.0f, 64.0f);
        myEnemy = new Enemy(500.0f, 400.0f);
        currentState = STATE_MENU;
        currentTool = 1;
        keysCollected = 0;
        keysNeeded = 0;
        currentLevel = 0;
    }

    ~TileMapScene() {
        delete myEnemy;
        delete myPlayer;
        delete myMap;
    }

    void setupCampaignLevel(int lvl) {
        currentLevel = lvl;
        keysCollected = 0;

        // Reset map to grass (1)
        for (int y = 0; y < 15; ++y) {
            for (int x = 0; x < 20; ++x) {
                myMap->setTile(x, y, 1);
            }
        }

        if (lvl == 1) {
            keysNeeded = 1;
            // Walls
            for(int x=5; x<15; ++x) myMap->setTile(x, 7, 2);
            // Key (Tile 3)
            myMap->setTile(10, 3, 3);
            // Exit (Tile 4)
            myMap->setTile(18, 13, 4);
            myPlayer->reset(64, 64);
            myEnemy->reset(500, 400);
        }
        else if (lvl == 2) {
            keysNeeded = 2;
            // Simple Maze
            for(int y=0; y<10; ++y) myMap->setTile(8, y, 2);
            for(int y=5; y<15; ++y) myMap->setTile(14, y, 2);
            // Keys
            myMap->setTile(2, 12, 3);
            myMap->setTile(18, 2, 3);
            // Exit
            myMap->setTile(18, 13, 4);
            myPlayer->reset(64, 64);
            myEnemy->reset(500, 100);
        }
        currentState = STATE_PLAY;
    }

    void paint(Vector2 pos) {
        if (currentState != STATE_EDITOR) return;
        int tileX = (int)(pos.x / 32.0f);
        int tileY = (int)(pos.y / 32.0f);
        myMap->setTile(tileX, tileY, currentTool);
    }

    virtual void render() {
        Renderer::get_singleton()->camera_2d_projection_set_to_window();

        if (currentState == STATE_PLAY) {
            float dt = ImGui::GetIO().DeltaTime;
            myPlayer->update(dt, myMap);
            myEnemy->update(dt, myMap, myPlayer);

            // Collision with Enemy
            if (myPlayer->position.distance_to(myEnemy->position) < 20.0f) {
                currentState = STATE_GAMEOVER;
            }

            // Tile interaction
            int tx = (int)(myPlayer->position.x / 32.0f);
            int ty = (int)(myPlayer->position.y / 32.0f);
            int currentTile = myMap->getTile(tx, ty);

            if (currentTile == 3) { // Collect Key
                myMap->setTile(tx, ty, 1);
                keysCollected++;
            }
            if (currentTile == 4 && keysCollected >= keysNeeded) { // Reach Exit
                if (currentLevel == 1) setupCampaignLevel(2);
                else currentState = STATE_WIN;
            }
        }

        if (currentState == STATE_PLAY || currentState == STATE_EDITOR) {
            myMap->render();
            if (currentState == STATE_PLAY) {
                myPlayer->render();
                myEnemy->render();
            }
        }

        Renderer::get_singleton()->camera_2d_reset();

        if (currentState == STATE_MENU) {
            ImGui::Begin("Main Menu");
            ImGui::Text("--- CAMPAIGN ---");
            if (ImGui::Button("Start Level 1")) setupCampaignLevel(1);

            ImGui::Separator();
            ImGui::Text("--- CUSTOM ---");
            if (ImGui::Button("Play Editor Map")) {
                currentLevel = 0;
                keysCollected = 0;
                keysNeeded = 0; // No keys needed for custom map by default
                myMap->load_map("level_data.dat");
                myPlayer->reset(100, 100);
                myEnemy->reset(400, 400);
                currentState = STATE_PLAY;
            }
            if (ImGui::Button("Level Editor")) {
                myMap->load_map("level_data.dat");
                currentState = STATE_EDITOR;
            }
            ImGui::End();
        }
        else if (currentState == STATE_EDITOR) {
            ImGui::Begin("Level Editor");
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
            ImGui::End();
        }
        else if (currentState == STATE_PLAY) {
            ImGui::Begin("Game Stats");
            if (currentLevel > 0) ImGui::Text("Level: %d", currentLevel);
            else ImGui::Text("Custom Map");

            ImGui::Text("Keys: %d / %d", keysCollected, keysNeeded);
            if (keysCollected >= keysNeeded && currentLevel > 0) {
                ImGui::TextColored(ImVec4(0,1,0,1), "EXIT OPEN!");
            }
            ImGui::Separator();
            if (myPlayer->dashActiveTimer > 0) ImGui::Text("DASH ACTIVE");
            else if (myPlayer->cooldownTimer > 0) ImGui::Text("Cooldown: %.1fs", myPlayer->cooldownTimer);
            else ImGui::Text("DASH READY (SPACE)");
            ImGui::End();
        }
        else if (currentState == STATE_GAMEOVER) {
            ImGui::Begin("Game Over");
            ImGui::Text("You were caught!");
            if (ImGui::Button("Retry")) {
                if (currentLevel == 0) {
                    myMap->load_map("level_data.dat");
                    myPlayer->reset(100, 100);
                    myEnemy->reset(400, 400);
                    currentState = STATE_PLAY;
                } else setupCampaignLevel(currentLevel);
            }
            if (ImGui::Button("Main Menu")) currentState = STATE_MENU;
            ImGui::End();
        }
        else if (currentState == STATE_WIN) {
            ImGui::Begin("Victory!");
            ImGui::Text("Campaign Completed!");
            if (ImGui::Button("Main Menu")) currentState = STATE_MENU;
            ImGui::End();
        }
    }
};

#endif
