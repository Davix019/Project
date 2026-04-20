#pragma once
#include "sfw.h"
#include "tilemap.h"
#include "player.h"

struct PathNode {
    int x, y;
    int parent_idx;
};

class EnemyBase {
public:
    Vector2 position;
    float speed;
    float size;
    Color color;
    bool active;
    Ref<Mesh> _mesh;
    Ref<ColorMaterial2D> _mat;

    EnemyBase() {
        active = false;
        _mesh.instance();
        _mat.instance();
    }

    virtual ~EnemyBase() {}

    void reset(float startX, float startY, bool isActive) {
        position = Vector2(startX, startY);
        active = isActive;
    }

    void update(float dt, TileMap* map, Player* player) {
        if (!active) return;

        int ex = (int)(position.x / 32.0f);
        int ey = (int)(position.y / 32.0f);
        int px = (int)(player->position.x / 32.0f);
        int py = (int)(player->position.y / 32.0f);

        Vector2 target_pos = player->position;

        if (ex != px || ey != py) {
            const int MAX_Q = 1024;
            PathNode queue[1024];
            int q_head = 0, q_tail = 0;

            const int WINDOW = 40;
            const int HALF = 20;
            bool visited[40][40];

            for (int i = 0; i < 40; ++i) {
                for (int j = 0; j < 40; ++j) {
                    visited[i][j] = false;
                }
            }

            queue[q_tail++] = {ex, ey, -1};
            visited[HALF][HALF] = true;

            int target_idx = -1;
            int dx[] = {0, 1, 0, -1, 1, -1, 1, -1};
            int dy[] = {-1, 0, 1, 0, 1, -1, -1, 1};

            while (q_head < q_tail) {
                PathNode curr = queue[q_head];
                if (curr.x == px && curr.y == py) {
                    target_idx = q_head;
                    break;
                }
                for (int i = 0; i < 8; ++i) {
                    int nx = curr.x + dx[i];
                    int ny = curr.y + dy[i];
                    int vx = nx - ex + HALF;
                    int vy = ny - ey + HALF;
                    if (vx >= 0 && vx < WINDOW && vy >= 0 && vy < WINDOW && !visited[vx][vy]) {
                        visited[vx][vy] = true;
                        int tile = map->getTile(nx, ny);
                        if (tile != 2 && tile != -1) {
                            if (i >= 4) {
                                if (map->getTile(curr.x, ny) == 2 || map->getTile(nx, curr.y) == 2) continue;
                            }
                            queue[q_tail++] = {nx, ny, q_head};
                            if (q_tail >= MAX_Q) break;
                        }
                    }
                }
                if (q_tail >= MAX_Q) break;
                q_head++;
            }
            if (target_idx != -1) {
                int curr_idx = target_idx;
                while (queue[curr_idx].parent_idx != -1 && queue[queue[curr_idx].parent_idx].parent_idx != -1) {
                    curr_idx = queue[curr_idx].parent_idx;
                }
                target_pos = Vector2(queue[curr_idx].x * 32.0f + 16.0f, queue[curr_idx].y * 32.0f + 16.0f);
            }
        }

        Vector2 dir = target_pos - position;
        if (dir.length_squared() > 1.0f) {
            position = position + dir.normalized() * speed * dt;
        }
    }

    void render() {
        if (!active) return;
        _mesh->clear();
        _mesh->vertex_dimesions = 2;
        _mesh->add_color(color);
        _mesh->add_vertex2(position.x - size/2, position.y - size/2);
        _mesh->add_color(color);
        _mesh->add_vertex2(position.x + size/2, position.y - size/2);
        _mesh->add_color(color);
        _mesh->add_vertex2(position.x + size/2, position.y + size/2);
        _mesh->add_color(color);
        _mesh->add_vertex2(position.x - size/2, position.y + size/2);
        _mesh->add_triangle(0, 2, 1);
        _mesh->add_triangle(0, 3, 2);
        _mesh->upload();
        _mat->bind();
        _mesh->render();
    }
};

class Scout : public EnemyBase {
public:
    Scout() { speed = 130.0f; size = 18.0f; color = Color(1.0f, 0.2f, 0.2f); }
};

class Tank : public EnemyBase {
public:
    Tank() { speed = 65.0f; size = 32.0f; color = Color(0.5f, 0.0f, 0.0f); }
};
