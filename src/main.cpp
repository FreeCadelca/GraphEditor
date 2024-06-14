#include "gtkmm.h"
#include "GraphEditorApp/GraphEditorApp.h"

/**
 * @brief Главная функция. Создаёт и запускает окно программы
 */
int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv);

    GraphEditorApp graphEditorApp;

    return app->run(graphEditorApp); // Вызов окна
}