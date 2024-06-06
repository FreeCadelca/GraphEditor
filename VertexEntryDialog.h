//
// Created by Dmitriy on 02.05.2024.
//

#ifndef GRAPHEDITOR_VERTEXENTRYDIALOG_H
#define GRAPHEDITOR_VERTEXENTRYDIALOG_H

#include <gtkmm.h>

class VertexEntryDialog : public Gtk::Dialog  {
private:
    Gtk::Label lbl;
    Gtk::Entry entry;
public:
    VertexEntryDialog(int center_x, int center_y);
    Glib::ustring get_text() const;
};


#endif //GRAPHEDITOR_VERTEXENTRYDIALOG_H
