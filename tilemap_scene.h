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
    STATE_GAMEOVER
};

class TileMapScene : public Scene {
    SFW_OBJECT(TileMapScene, Scene);

public:
    TileMap* myMap;
    Player* myPlayer;
    Enemy* myEnemy;
    GameState currentState;
    int currentTool;
    int lastClickedX;
    int lastClickedY;

    int score;
    int highScore;
    float scoreTimer;

    TileMapScene() {
        myMap = new TileMap(20, 15);
        myPlayer = new Player(100.0f, 100.0f);
        myEnemy = new Enemy(400.0f, 400.0f);
        currentState = STATE_MENU;
        currentTool = 1;
        lastClickedX = -1;
        lastClickedY = -1;

        score = 0;
        highScore = 0;
        scoreTimer = 0.0f;

        loadHighScore();
    }

    ~TileMapScene() {
        delete myEnemy;
        delete myPlayer;
        delete myMap;
    }

    void loadHighScore() {
        FileAccessRef file = FileAccess::create_and_open("highscore.dat", FileAccess::READ);
        if (file.is_valid()) {
            highScore = file->get_32();
            file->close();
        }
    }

    void saveHighScore() {
        FileAccessRef file = FileAccess::create_and_open("highscore.dat", FileAccess::WRITE);
        if (file.is_valid()) {
            file->store_32(highScore);
            file->close();
        }
    }

    void paint(Vector2 pos) {
        if (currentState != STATE_EDITOR) return;

        int tileX = (int)(pos.x / 32.0f);
        int tileY = (int)(pos.y / 32.0f);

        lastClickedX = tileX;
        lastClickedY = tileY;

        myMap->setTile(tileX, tileY, currentTool);
    }

    virtual void render() {
        Renderer::get_singleton()->camera_2d_projection_set_to_window();

        if (currentState == STATE_PLAY) {
            float dt = 0.016f;

            scoreTimer += dt;
            if (scoreTimer >= 1.0f) {
                score += 10;
                scoreTimer = 0.0f;
            }

            myPlayer->update(dt, myMap);
            myEnemy->update(dt, myMap, myPlayer);

            if (myPlayer->position.distance_to(myEnemy->position) < 20.0f) {
                currentState = STATE_GAMEOVER;
                if (score > highScore) {
                    highScore = score;
                    saveHighScore();
                }
            }
        }

        if (currentState == STATE_PLAY || currentState == STATE_EDITOR) {
            myMap->render();
        }

        if (currentState == STATE_PLAY) {
            myPlayer->render();
            myEnemy->render();
        }

        Renderer::get_singleton()->camera_2d_reset();

        if (currentState == STATE_MENU) {
            ImGui::Begin("Fomenu");
            ImGui::Text("Legjobb pontszam: %d", highScore);
            ImGui::Separator();
            if (ImGui::Button("Jatek Inditasa")) {
                myPlayer->reset(100.0f, 100.0f);
                myEnemy->reset(400.0f, 400.0f);
                score = 0;
                scoreTimer = 0.0f;
                myMap->load_map("level_data.dat");
                currentState = STATE_PLAY;
            }
            if (ImGui::Button("Palya Szerkeszto")) {
                myMap->load_map("level_data.dat");
                currentState = STATE_EDITOR;
            }
            ImGui::End();
        }
        else if (currentState == STATE_EDITOR) {
            ImGui::Begin("TileMap Szerkeszto");
            if (ImGui::Button("Vissza a Menube")) currentState = STATE_MENU;
            ImGui::Separator();
            if (ImGui::Button("Fu (Zold) - 1")) currentTool = 1;
            if (ImGui::Button("Fal (Szurke) - 2")) currentTool = 2;
            if (ImGui::Button("Torles (Ures) - 0")) currentTool = 0;
            ImGui::Separator();
            if (ImGui::Button("Palya Mentese")) myMap->save_map("level_data.dat");
            ImGui::SameLine();
            if (ImGui::Button("Palya Betoltese")) myMap->load_map("level_data.dat");
            ImGui::End();
        }
        else if (currentState == STATE_PLAY) {
            ImGui::Begin("Jatek");
            ImGui::Text("Pontszam: %d", score);
            ImGui::Text("Kepesseg: SPACE (Gyorsitas)");
            ImGui::End();
        }
        else if (currentState == STATE_GAMEOVER) {
            ImGui::Begin("Game Over");
            ImGui::Text("Elkaptak!");
            ImGui::Text("Elert pontszam: %d", score);
            ImGui::Separator();
            if (ImGui::Button("Vissza a Menube")) currentState = STATE_MENU;
            ImGui::End();
        }
    }
};

#endif
