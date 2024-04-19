//
// Created by Dmitriy on 19.04.2024.
//

#include "GraphEditorApp.h"
#include <iostream>
#include "Canvas.h"

GraphEditorApp::GraphEditorApp() : ui{Gtk::Builder::create_from_file("design_new.glade")} {
    if (ui) {
        ui->get_widget<Gtk::Box>("main_box", this->main_box);
        this->printed_graph_label = Glib::RefPtr<Gtk::Label>::cast_dynamic(ui->get_object("printed_graph_label"));
        this->canvas = new Canvas(this->printed_graph_label);
        this->main_box->add(*canvas);
        canvas->show();

        this->tool_choose_color = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(ui->get_object("tool_choose_color"));
        //выцепление виджета инструмента выбора цвета
        this->tool_choose_color->signal_clicked().connect(sigc::mem_fun(*this->canvas, &Canvas::choose_color));
        //подключение функционала при нажатии

        this->tool_add_vertex = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(ui->get_object("tool_add_vertex"));
        //выцепление виджета инструмента рисования вершин
        //подключение функционала при нажатии
        this->tool_add_vertex->signal_clicked().connect(
                sigc::bind(sigc::mem_fun(*this->canvas, &Canvas::change_tool), Canvas::VERTEX)
        );

        this->tool_add_edge = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(ui->get_object("tool_add_edge"));
        //выцепление виджета инструмента рисования рёбер
        this->tool_add_edge->signal_clicked().connect(
                sigc::bind(sigc::mem_fun(*this->canvas, &Canvas::change_tool), Canvas::EDGE)
        );

        this->print_graph_button = Glib::RefPtr<Gtk::Button>::cast_dynamic(ui->get_object("print_graph_button"));
        //выцепление виджета кнопки разворачивания распечатки графа
        this->print_graph_button->signal_clicked().connect(
                sigc::bind(sigc::mem_fun(*this->canvas,&Canvas::print_graph),this->print_graph_button)
        );

        this->entry_for_weight = Glib::RefPtr<Gtk::Entry>::cast_dynamic(ui->get_object("entry_for_weight"));
        this->entry_for_weight->signal_activate().connect(sigc::mem_fun(*this, &GraphEditorApp::on_change_weight_release));

        this->set_resize_mode(Gtk::RESIZE_QUEUE);
        this->add(*this->main_box);
//        show_all();
    }
}

void GraphEditorApp::on_change_weight_release() {
    std::cout << "Yess!";
};