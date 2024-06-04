//
// Created by Dmitriy on 02.05.2024.
//

#include "VertexEntryDialog.h"

VertexEntryDialog::VertexEntryDialog() : lbl("Please enter the weight:")  {
    get_content_area()->pack_start(lbl);
    get_content_area()->pack_start(entry);
    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    show_all_children();
}

Glib::ustring VertexEntryDialog::get_text() const {
    return entry.get_text();
}