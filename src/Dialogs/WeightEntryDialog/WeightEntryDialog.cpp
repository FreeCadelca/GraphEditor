/**
 * @file WeightEntryDialog
 * @brief Реализация методов класса WeightEntryDialog.
 *
 * * @authors
 * Дмитрий Кулешов
 */

#include "WeightEntryDialog.h"

/**
 * @brief Конструктор, вызывающий диалог с центром в указанных координатах
 *
 * @param center_x центр диалога по оси оX
 *
 * @param center_y центр диалога по оси оY
 */
WeightEntryDialog::WeightEntryDialog(int center_x, int center_y) : lbl("Please enter the weight:") {
    get_content_area()->pack_start(lbl);
    this->entry.set_max_length(9);
    get_content_area()->pack_start(entry);
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    show_all_children();

    int size_x, size_y;
    this->get_size(size_x, size_y);
    move(center_x - size_x / 2, center_y - size_y / 2);
}

/**
 * @brief метод извлечения текста из поля ввода
 *
 * @return строка из поля ввода
 */
Glib::ustring WeightEntryDialog::get_text() const {
    return entry.get_text();
}