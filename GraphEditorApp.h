//
// Created by Dmitriy on 19.04.2024.
//

#ifndef GRAPHEDITOR_GRAPHEDITORAPP_H
#define GRAPHEDITOR_GRAPHEDITORAPP_H

#include <gtkmm.h>
#include <iostream>
#include "Canvas.h"
#include "Graph.h"
#include "VertexEntryDialog.h"
#include "WeightEntryDialog.h"
#include <string>


class GraphEditorApp : public Gtk::ApplicationWindow {
    Gtk::Box *main_box;
    int nextWeight = 1;

    Glib::RefPtr<Gtk::Label> printed_graph_label_left;
    Glib::RefPtr<Gtk::Label> printed_graph_label_right;
    Glib::RefPtr<Gtk::Label> printed_algorithm_label;
    Glib::RefPtr<Gtk::ToolButton> tool_choose_color;
    Glib::RefPtr<Gtk::ToolButton> tool_add_vertex;
    Glib::RefPtr<Gtk::ToolButton> tool_add_edge;
    Glib::RefPtr<Gtk::ToolButton> tool_change_weight;
    Glib::RefPtr<Gtk::Button> print_graph_button;
    Glib::RefPtr<Gtk::Button> run_algorithm_button;
    Glib::RefPtr<Gtk::Entry> entry_for_weight;
    Glib::RefPtr<Gtk::ComboBox> choose_algorithm_cb;
    std::string handling_new_weight(Glib::ustring new_weight);

    Glib::RefPtr<Gtk::Builder> ui;
public:
    GraphEditorApp();

    void on_change_weight();

    void print_graph_data();

    void print_algorithm();
};

//
#endif //GRAPHEDITOR_GRAPHEDITORAPP_H
