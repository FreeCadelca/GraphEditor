//
// Created by Dmitriy on 30.04.2024.
//

#ifndef GRAPHEDITOR_CHOOSEWEIGHTAPP_H
#define GRAPHEDITOR_CHOOSEWEIGHTAPP_H

#include <gtkmm.h>


class ChooseWeightApp  : public Gtk::ApplicationWindow  {
    Gtk::Box *main_box;

    Glib::RefPtr<Gtk::Entry> weight_entry;

    Glib::RefPtr<Gtk::Button> button_cancel;
    Glib::RefPtr<Gtk::Button> button_ok;

    Glib::RefPtr<Gtk::Builder> ui;
public:
    ChooseWeightApp();
    void on_ok_btn_clicked();
    void on_cancel_btn_clicked();
};


#endif //GRAPHEDITOR_CHOOSEWEIGHTAPP_H
