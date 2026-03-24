#include "tilemap_application.h"

int main(int argc, char **argv) {
    Application *application = memnew(TileMapApplication());

    application->start_main_loop();

    memdelete(application);

    return 0;
}
