#pragma once
#include "sfw.h"
#include "tilemap.h"

class Player {
public:
    Vector2 position;
    float speed;
    Ref<Mesh> _mesh;
    Ref<ColorMaterial2D> _mat;

    Player(float startX, float startY) {
        position = Vector2(startX, startY);
        speed = 250.0f;
        _mesh.instance();
        _mat.instance();
    }

    void update(float dt, TileMap* map) {
        Vector2 dir(0, 0);
        Input* input = Input::get_singleton();

        if (input->is_key_pressed(KEY_W)) dir.y -= 1;
        if (input->is_key_pressed(KEY_S)) dir.y += 1;
        if (input->is_key_pressed(KEY_A)) dir.x -= 1;
        if (input->is_key_pressed(KEY_D)) dir.x += 1;

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

        _mesh->add_color(Color(0, 0, 1));
        _mesh->add_vertex2(position.x - size/2, position.y - size/2);
        _mesh->add_color(Color(0, 0, 1));
        _mesh->add_vertex2(position.x + size/2, position.y - size/2);
        _mesh->add_color(Color(0, 0, 1));
        _mesh->add_vertex2(position.x + size/2, position.y + size/2);
        _mesh->add_color(Color(0, 0, 1));
        _mesh->add_vertex2(position.x - size/2, position.y + size/2);

        _mesh->add_triangle(0, 2, 1);
        _mesh->add_triangle(0, 3, 2);
        _mesh->upload();

        _mat->bind();
        _mesh->render();
    }
};
