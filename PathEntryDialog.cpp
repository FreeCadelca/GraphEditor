//
// Created by Dmitriy on 06.06.2024.
//

#include "PathEntryDialog.h"

PathEntryDialog::PathEntryDialog() {
//    this->lbl_question.set_label("Enter the vertices of the beginning and end of the path");
//    get_content_area()->pack_start(lbl_question);

    this->lbl_left.set_label("From where?");
    this->lbl_right.set_label("Where to?");

    auto* hbox_labels = new Gtk::HBox;
    hbox_labels->pack_start(lbl_left);
    hbox_labels->pack_start(lbl_right);
    get_content_area()->pack_start(*hbox_labels);

    auto* hbox_entries = new Gtk::HBox;
    entry1.set_max_length(1);
    hbox_entries->pack_start(entry1);
    entry2.set_max_length(1);
    hbox_entries->pack_start(entry2);
    get_content_area()->pack_start(*hbox_entries);

    add_button("OK", Gtk::RESPONSE_OK);
    add_button("Cancel", Gtk::RESPONSE_CANCEL);
    show_all_children();
}

Glib::ustring PathEntryDialog::get_vertex_from() const {
    return entry1.get_text();
}

Glib::ustring PathEntryDialog::get_vertex_to() const {
    return entry2.get_text();
}