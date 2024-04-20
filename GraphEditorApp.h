//
// Created by Dmitriy on 19.04.2024.
//

#ifndef GRAPHEDITOR_GRAPHEDITORAPP_H
#define GRAPHEDITOR_GRAPHEDITORAPP_H

#include <gtkmm.h>
#include "Canvas.h"

class GraphEditorApp : public Gtk::ApplicationWindow  {
    Gtk::Box *main_box;
    Canvas *canvas;
    int nextWeight = 1;

    Glib::RefPtr<Gtk::Label> printed_graph_label_left;
    Glib::RefPtr<Gtk::Label> printed_graph_label_right;
    Glib::RefPtr<Gtk::ToolButton> tool_choose_color;
    Glib::RefPtr<Gtk::ToolButton> tool_add_vertex;
    Glib::RefPtr<Gtk::ToolButton> tool_add_edge;
    Glib::RefPtr<Gtk::Button> print_graph_button;
    Glib::RefPtr<Gtk::Entry> entry_for_weight;

    Glib::RefPtr<Gtk::Builder> ui;
public:
    GraphEditorApp();
    void on_change_weight_release();
};


#endif //GRAPHEDITOR_GRAPHEDITORAPP_H
