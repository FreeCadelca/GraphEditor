//
// Created by Dmitriy on 02.05.2024.
//

#include "WeightEntryDialog.h"

WeightEntryDialog::WeightEntryDialog() : lbl("Please enter text:")  {
    get_content_area()->pack_start(lbl);
    get_content_area()->pack_start(entry);
    add_button("OK", Gtk::RESPONSE_OK);
    show_all_children();
}

Glib::ustring WeightEntryDialog::get_text() const {
    return entry.get_text();
}