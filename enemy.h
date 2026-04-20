#pragma once
#include "sfw.h"
#include "tilemap.h"
#include "player.h"

class Enemy {
public:
    Vector2 position;
    float speed;
    Ref<Mesh> _mesh;
    Ref<ColorMaterial2D> _mat;

    Enemy(float startX, float startY) {
        position = Vector2(startX, startY);
        speed = 120.0f;
        _mesh.instance();
        _mat.instance();
    }

    void reset(float startX, float startY) {
        position = Vector2(startX, startY);
    }

    void update(float dt, TileMap* map, Player* player) {
        Vector2 dir = player->position - position;

        if (dir.length_squared() > 0) {
            dir.normalize();
            Vector2 newPos = position + dir * speed * dt;

            int tileX = (int)(newPos.x / 32.0f);
            int tileY = (int)(newPos.y / 32.0f);

            if (map->getTile(tileX, tileY) != 2) {
                position = newPos;
            }
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
