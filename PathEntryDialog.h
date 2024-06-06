//
// Created by Dmitriy on 06.06.2024.
//

#ifndef GRAPHEDITOR_PATHENTRYDIALOG_H
#define GRAPHEDITOR_PATHENTRYDIALOG_H

#include <gtkmm.h>

class PathEntryDialog : public Gtk::Dialog  {
private:
//    Gtk::Label lbl_question;
    Gtk::Label lbl_left;
    Gtk::Label lbl_right;
    Gtk::Entry entry1;
    Gtk::Entry entry2;
public:
    PathEntryDialog(int center_x, int center_y);
    Glib::ustring get_vertex_from() const;
    Glib::ustring get_vertex_to() const;
};



#endif //GRAPHEDITOR_PATHENTRYDIALOG_H
