//
// Created by Dmitriy on 02.06.2024.
//

#include "WeightEntryDialog.h"

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

Glib::ustring WeightEntryDialog::get_text() const {
    return entry.get_text();
}