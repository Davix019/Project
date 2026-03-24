#include "tilemap_application.h"

TileMapApplication::TileMapApplication() {
    Renderer::initialize();
    GUI::initialize();

    myScene = memnew(TileMapScene);
    scene = Ref<Scene>(myScene);
}

TileMapApplication::~TileMapApplication() {
    GUI::destroy();
    Renderer::destroy();
}

void TileMapApplication::input_event(const Ref<InputEvent> &event) {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }

    Ref<InputEventMouseButton> mb = event;
    if (mb.is_valid() && mb->get_button_index() == 1 && mb->is_pressed()) {
        myScene->paint(mb->get_position());
    }

    Ref<InputEventMouseMotion> mm = event;
    if (mm.is_valid() && (mm->get_button_mask() & 1)) {
        myScene->paint(mm->get_position());
    }

    Application::input_event(event);
}

void TileMapApplication::render() {
    Renderer::get_singleton()->clear_screen(Color(0.2, 0.2, 0.2, 1.0));

    GUI::new_frame();
    Application::render();
    GUI::render();
}
