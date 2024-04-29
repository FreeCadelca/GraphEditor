//
// Created by Dmitriy on 19.04.2024.
//

#include "GraphEditorApp.h"
#include <iostream>
#include "Canvas.h"


GraphEditorApp::GraphEditorApp() : ui{Gtk::Builder::create_from_file("design_new.glade")} {
    if (ui) {
        ui->get_widget<Gtk::Box>("main_box", this->main_box);
        this->printed_graph_label_left = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_graph_label_left")
        );
        this->printed_graph_label_right = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_graph_label_right")
        );
        this->printed_algorithm_label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_algorithm_label")
        );
        this->canvas = new Canvas();
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
                sigc::mem_fun(*this, &GraphEditorApp::print_graph_data)
        );

        this->run_algorithm_button = Glib::RefPtr<Gtk::Button>::cast_dynamic(ui->get_object("run_algorithm_button"));
        //выцепление виджета кнопки разворачивания распечатки графа
        this->run_algorithm_button->signal_clicked().connect(
                sigc::mem_fun(*this, &GraphEditorApp::print_algorithm)
        );

        this->entry_for_weight = Glib::RefPtr<Gtk::Entry>::cast_dynamic(ui->get_object("entry_for_weight"));
//        this->entry_for_weight->signal_activate().connect(sigc::mem_fun(*this, &GraphEditorApp::on_change_weight_release));
        this->entry_for_weight->signal_changed().connect(
                sigc::mem_fun(*this, &GraphEditorApp::on_change_weight_release));

        this->choose_algorithm_cb = Glib::RefPtr<Gtk::ComboBox>::cast_dynamic(ui->get_object("choose_algorithm_cb"));

        this->set_resize_mode(Gtk::RESIZE_QUEUE);
        this->add(*this->main_box);
//        show_all();
    }
}

void GraphEditorApp::on_change_weight_release() {
    Glib::ustring new_weight = this->entry_for_weight->get_text();
//    std::cout << new_weight << "\n";
    for (auto i: new_weight) std::cout << i << " ";
    std::cout << '\n';
    bool is_number = true;
    for (auto i: new_weight) {
        if (i >= 58 or i <= 47) {
            is_number = false;
            break;
        }
    }
    if (new_weight.empty()) {
        is_number = false;
    }
    if (is_number) {
        std::cout << new_weight << "\n";
        this->canvas->graph->nextWeight = std::stoi(new_weight);
    } else {
        this->entry_for_weight->set_text(std::to_string(this->nextWeight));
    }
};
//
void GraphEditorApp::print_graph_data() {//функция распечатывания графа
    if (this->print_graph_button->get_label() == "Print Graph") {//"развёртывание" лейбла с распечаткой
        this->printed_graph_label_left->set_text(this->canvas->graph->getPrintoutAdjList());
        this->printed_graph_label_right->set_text(this->canvas->graph->getPrintoutAdjMatrix());
        this->printed_graph_label_left->show();
        this->printed_graph_label_right->show();
        this->print_graph_button->set_label("Close printout");
    } else { //"свёртывание" лейбла с распечаткой
        this->printed_graph_label_left->hide();
        this->printed_graph_label_right->hide();
        this->print_graph_button->set_label("Print Graph");
    }
}

void GraphEditorApp::print_algorithm() {//функция распечатывания алгоритма на графе
    if (this->run_algorithm_button->get_label() == "Run algorithm") { //"развёртывание" лейбла с распечаткой
        std::string algorithm = this->choose_algorithm_cb->get_active_id();
        this->canvas->graph->runAlgorithm(this->choose_algorithm_cb->get_active_id());
        this->printed_algorithm_label->set_text(this->canvas->graph->getPrintoutAlgorithm());
        this->printed_algorithm_label->show();
        this->run_algorithm_button->set_label("Close algorithm");
    } else { //"свёртывание" лейбла с распечаткой
        this->printed_algorithm_label->hide();
        this->printed_algorithm_label->hide();
        this->run_algorithm_button->set_label("Run algorithm");
    }
}

