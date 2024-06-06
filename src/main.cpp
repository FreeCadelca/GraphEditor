#include "gtkmm.h"
#include "GraphEditorApp/GraphEditorApp.h"


int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv);

    GraphEditorApp graphEditorApp;

    return app->run(graphEditorApp); // Вызов окна
}