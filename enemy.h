#pragma once
#include "sfw.h"
#include "tilemap.h"
#include "player.h"

class Enemy {
public:
    Vector2 position;
    Vector2 lastPosition;
    float speed;
    float stuckTimer;
    float sideStepTimer;
    Vector2 sideDir;

    Ref<Mesh> _mesh;
    Ref<ColorMaterial2D> _mat;

    Enemy(float startX, float startY) {
        position = Vector2(startX, startY);
        lastPosition = position;
        speed = 120.0f;
        stuckTimer = 0.0f;
        sideStepTimer = 0.0f;
        _mesh.instance();
        _mat.instance();
    }

    void reset(float startX, float startY) {
        position = Vector2(startX, startY);
        lastPosition = position;
        stuckTimer = 0.0f;
        sideStepTimer = 0.0f;
    }

    void update(float dt, TileMap* map, Player* player) {
        if (position.distance_to(lastPosition) < 0.5f) {
            stuckTimer += dt;
        } else {
            stuckTimer = 0.0f;
        }
        lastPosition = position;

        Vector2 dir;
        if (stuckTimer > 0.5f && sideStepTimer <= 0.0f) {
            sideStepTimer = 0.8f;
            Vector2 toPlayer = (player->position - position).normalized();
            sideDir = Vector2(-toPlayer.y, toPlayer.x);
        }

        if (sideStepTimer > 0.0f) {
            sideStepTimer -= dt;
            dir = sideDir;
        } else {
            dir = (player->position - position).normalized();
        }

        Vector2 newPos = position + dir * speed * dt;
        int tileX = (int)(newPos.x / 32.0f);
        int tileY = (int)(newPos.y / 32.0f);

        if (map->getTile(tileX, tileY) != 2) {
            position = newPos;
        } else {
            sideStepTimer = 0.5f;
            sideDir = -sideDir;
        }
    }

    void render() {
        _mesh->clear();
        _mesh->vertex_dimesions = 2;
        float size = 20.0f;
        _mesh->add_color(Color(1, 0, 0));
        _mesh->add_vertex2(position.x - size/2, position.y - size/2);
        _mesh->add_color(Color(1, 0, 0));
        _mesh->add_vertex2(position.x + size/2, position.y - size/2);
        _mesh->add_color(Color(1, 0, 0));
        _mesh->add_vertex2(position.x + size/2, position.y + size/2);
        _mesh->add_color(Color(1, 0, 0));
        _mesh->add_vertex2(position.x - size/2, position.y + size/2);
        _mesh->add_triangle(0, 2, 1);
        _mesh->add_triangle(0, 3, 2);
        _mesh->upload();
        _mat->bind();
        _mesh->render();
    }
};
