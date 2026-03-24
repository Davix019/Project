#ifndef TILEMAP_APPLICATION_H
#define TILEMAP_APPLICATION_H

#include "sfw.h"
#include "tilemap.h"

class TileMapApplication : public Application {
    SFW_OBJECT(TileMapApplication, Application);

public:
    virtual void render();

    TileMapApplication();
    ~TileMapApplication();

protected:
    TileMap* myMap;
};

#endif
