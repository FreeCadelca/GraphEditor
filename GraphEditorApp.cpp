//
// Created by Dmitriy on 19.04.2024.
//

#include "GraphEditorApp.h"

GraphEditorApp::GraphEditorApp() : ui{Gtk::Builder::create_from_file("design_new.glade")} {
    if (ui) {
        ui->get_widget<Gtk::Box>("main_box", this->main_box);
        this->main_box->add(*Canvas::getInstance());
        Canvas::getInstance()->show();

        this->tool_choose_color = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(
                ui->get_object("tool_choose_color")
        );
        this->tool_choose_color->signal_clicked().connect(
                sigc::mem_fun(*Canvas::getInstance(), &Canvas::choose_color)
        );

        this->tool_add_vertex = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(
                ui->get_object("tool_add_vertex")
        );
        this->tool_add_vertex->signal_clicked().connect(
                sigc::bind(
                        sigc::mem_fun(*Canvas::getInstance(), &Canvas::change_tool),
                        Canvas::VERTEX
                )
        );

        this->tool_add_edge = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(
                ui->get_object("tool_add_edge")
        );
        this->tool_add_edge->signal_clicked().connect(
                sigc::bind(
                        sigc::mem_fun(*Canvas::getInstance(), &Canvas::change_tool),
                        Canvas::EDGE
                )
        );

        this->print_graph_button = Glib::RefPtr<Gtk::Button>::cast_dynamic(
                ui->get_object("print_graph_button")
        );
        this->print_graph_button->signal_clicked().connect(
                sigc::mem_fun(*this, &GraphEditorApp::print_graph_data)
        );

        this->run_algorithm_button = Glib::RefPtr<Gtk::Button>::cast_dynamic(
                ui->get_object("run_algorithm_button")
        );
        this->run_algorithm_button->signal_clicked().connect(
                sigc::mem_fun(*this, &GraphEditorApp::print_algorithm)
        );

        this->entry_for_weight = Glib::RefPtr<Gtk::Entry>::cast_dynamic(
                ui->get_object("entry_for_weight")
        );
        this->entry_for_weight->signal_changed().connect(
                sigc::mem_fun(*this, &GraphEditorApp::on_change_weight_release)
        );

        this->choose_algorithm_cb = Glib::RefPtr<Gtk::ComboBox>::cast_dynamic(
                ui->get_object("choose_algorithm_cb")
        );

        this->printed_graph_label_left = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_graph_label_left")
        );

        this->printed_graph_label_right = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_graph_label_right")
        );

        this->printed_algorithm_label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("printed_algorithm_label")
        );

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
        Graph::getInstance()->nextWeight = std::stoi(new_weight);
    } else {
        this->entry_for_weight->set_text(std::to_string(this->nextWeight));
    }
}

void GraphEditorApp::print_graph_data() {
    if (this->print_graph_button->get_label() == "Print Graph") {
        this->printed_graph_label_left->set_text(Graph::getInstance()->getPrintoutAdjList());
        this->printed_graph_label_right->set_text(Graph::getInstance()->getPrintoutAdjMatrix());
        this->printed_graph_label_left->show();
        this->printed_graph_label_right->show();
        this->print_graph_button->set_label("Close printout");
    } else {
        this->printed_graph_label_left->hide();
        this->printed_graph_label_right->hide();
        this->print_graph_button->set_label("Print Graph");
    }
}

void GraphEditorApp::print_algorithm() {
    if (this->run_algorithm_button->get_label() == "Run algorithm") {
        if (this->choose_algorithm_cb->get_active_id() == "DFS"
        or this->choose_algorithm_cb->get_active_id() == "BFS"
        or this->choose_algorithm_cb->get_active_id() == "Bellman-Ford"
        or this->choose_algorithm_cb->get_active_id() == "Djkstra") {
            std::string algorithm = this->choose_algorithm_cb->get_active_id();
            WeightEntryDialog dialog;
            int result = dialog.run();
            if (result == Gtk::RESPONSE_OK) {
                std::cout << "Entered text: " << dialog.get_text() << std::endl;
            } else {
                std::cout << "Canceled" << std::endl;
                return;
            }
            Graph::getInstance()->runAlgorithm(
                    this->choose_algorithm_cb->get_active_id(),
                    (char) dialog.get_text()[0]
            );
        } else {
            Graph::getInstance()->runAlgorithm(this->choose_algorithm_cb->get_active_id());
        }
        this->printed_algorithm_label->set_text(Graph::getInstance()->getPrintoutAlgorithm());
        this->printed_algorithm_label->show();
        this->run_algorithm_button->set_label("Close algorithm");
    } else {
        this->printed_algorithm_label->hide();
        this->printed_algorithm_label->hide();
        this->run_algorithm_button->set_label("Run algorithm");
    }
}

