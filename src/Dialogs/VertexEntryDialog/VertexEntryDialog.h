/**
 * @file VertexEntryDialog.h
 * @brief Заголовочный файл класса VertexEntryDialog.
 *
 * * @authors
 * Дмитрий Кулешов
 */

#ifndef GRAPHEDITOR_VERTEXENTRYDIALOG_H
#define GRAPHEDITOR_VERTEXENTRYDIALOG_H

#include <gtkmm.h>

/**
 * @brief Класс, реализующий диалог ввода одной вершины
 */
class VertexEntryDialog : public Gtk::Dialog  {
private:
    Gtk::Label lbl; ///< Ярлык с запросом
    Gtk::Entry entry; ///< Поле для ввода
public:
    VertexEntryDialog(int center_x, int center_y);
    Glib::ustring get_text() const;
};


#endif //GRAPHEDITOR_VERTEXENTRYDIALOG_H
