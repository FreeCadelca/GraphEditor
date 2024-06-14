//
// Created by Dmitriy on 06.06.2024.
//

#ifndef GRAPHEDITOR_PATHENTRYDIALOG_H
#define GRAPHEDITOR_PATHENTRYDIALOG_H

#include <gtkmm.h>

/**
 * @brief Класс, реализующий диалог ввода двух вершин
 */
class PathEntryDialog : public Gtk::Dialog {
private:
    Gtk::Label lbl_left; ///< Левый ярлык
    Gtk::Label lbl_right; ///< Правый ярлык
    Gtk::Entry entry1; ///< Левое поле ввода
    Gtk::Entry entry2; ///< Правое поле ввода
public:
    PathEntryDialog(int center_x, int center_y);

    Glib::ustring get_vertex_from() const;

    Glib::ustring get_vertex_to() const;
};


#endif //GRAPHEDITOR_PATHENTRYDIALOG_H
