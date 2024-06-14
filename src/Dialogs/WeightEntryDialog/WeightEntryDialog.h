//
// Created by Dmitriy on 02.06.2024.
//

#ifndef GRAPHEDITOR_WEIGHTENTRYDIALOG_H
#define GRAPHEDITOR_WEIGHTENTRYDIALOG_H

#include <gtkmm.h>

/**
 * @brief Класс, реализующий диалог для ввода веса
 */
class WeightEntryDialog : public Gtk::Dialog {
private:
    Gtk::Label lbl; ///< Ярлык с запросом
    Gtk::Entry entry; ///< Поле для ввода
public:
    WeightEntryDialog(int center_x, int center_y);

    Glib::ustring get_text() const;
};


#endif //GRAPHEDITOR_WEIGHTENTRYDIALOG_H
