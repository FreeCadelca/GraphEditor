//
// Created by Dmitriy on 19.04.2024.
//

#ifndef GRAPHEDITOR_GRAPHEDITORAPP_H
#define GRAPHEDITOR_GRAPHEDITORAPP_H

#include <gtkmm.h>
#include <iostream>
#include <string>
#include "../../src/Canvas/Canvas.h"
#include "../Graph/Graph.h"
#include "../Dialogs/VertexEntryDialog/VertexEntryDialog.h"
#include "../Dialogs/WeightEntryDialog/WeightEntryDialog.h"
#include "../Dialogs/PathEntryDialog/PathEntryDialog.h"
#include <string>

/**
 * @brief Класс, реализующий главное окно программы.
 */
class GraphEditorApp : public Gtk::ApplicationWindow {
    Gtk::Box *main_box; ///< Указатель на главный контейнер окна
    Glib::RefPtr<Gtk::Label> printed_graph_label_left; ///< Указатель на левый ярлык распечатки данных графа
    Glib::RefPtr<Gtk::Label> printed_graph_label_right; ///< Указатель на правый ярлык распечатки данных графа
    Glib::RefPtr<Gtk::Label> printed_algorithm_label; ///< Указатель на ярлык распечатки информации об алгоритме
    Glib::RefPtr<Gtk::ToolButton> tool_choose_color; ///< Указатель на кнопку-инструмент смены цвета
    Glib::RefPtr<Gtk::ToolButton> tool_add_vertex; ///< Указатель на кнопку-инструмент добавления вершины
    Glib::RefPtr<Gtk::ToolButton> tool_add_edge; ///< Указатель на кнопку-инструмент добавления ребра
    Glib::RefPtr<Gtk::ToolButton> tool_change_weight; ///< Указатель на кнопку-инструмент смены веса
    Glib::RefPtr<Gtk::Label> next_weight_label; ///< Указатель на ярлык следующего веса
    Glib::RefPtr<Gtk::Button> print_graph_button; ///< Указатель на кнопку распечатки данных графа
    Glib::RefPtr<Gtk::Button> run_algorithm_button; ///< Указатель на кнопку распечатки информации об алгоритме
    Glib::RefPtr<Gtk::ComboBox> choose_algorithm_cb; ///< Указатель на комбо-бокс выбора алгоритма

    Glib::RefPtr<Gtk::Builder> ui; ///< Указатель на построителя интерфейса
    /**
     * @brief Метод запуска ошибки с сообщением.
     */
    void run_error_dialog(const std::string &message);

public:
    GraphEditorApp();

    void on_change_weight();

    void print_graph_data();

    void print_algorithm(const std::string &test_input = "");

    std::pair<int, int> getCenterOfWindow();

    std::string handling_new_weight(Glib::ustring new_weight);

    void setAlgorithmName(const std::string &algorithm);

    std::string getAlgorithmName() const;
};

//
#endif //GRAPHEDITOR_GRAPHEDITORAPP_H
