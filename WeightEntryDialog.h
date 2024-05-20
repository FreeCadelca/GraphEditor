//
// Created by Dmitriy on 02.05.2024.
//

#ifndef GRAPHEDITOR_WEIGHTENTRYDIALOG_H
#define GRAPHEDITOR_WEIGHTENTRYDIALOG_H

#include <gtkmm.h>

class WeightEntryDialog : public Gtk::Dialog  {
private:
    Gtk::Label lbl;
    Gtk::Entry entry;
public:
    WeightEntryDialog();
    Glib::ustring get_text() const;
};


#endif //GRAPHEDITOR_WEIGHTENTRYDIALOG_H
