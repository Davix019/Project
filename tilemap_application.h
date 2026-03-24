#ifndef TILEMAP_APPLICATION_H
#define TILEMAP_APPLICATION_H

#include "sfw.h"
#include "tilemap_scene.h"

class TileMapApplication : public Application {
    SFW_OBJECT(TileMapApplication, Application);

public:
    virtual void input_event(const Ref<InputEvent> &event);
    virtual void render();

    TileMapApplication();
    ~TileMapApplication();

protected:
    TileMapScene* myScene;
};

#endif
