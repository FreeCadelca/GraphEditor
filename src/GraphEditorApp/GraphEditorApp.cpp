/**
 * @file GraphEditorApp.cpp
 * @brief Реализация методов класса GraphEditorApp.
 *
 * * @authors
 * Дмитрий Кулешов
 */
#include "GraphEditorApp.h"

/**
 * @brief Конструктор класса Graph.
 * Создаёт окно программы, собирая все объекты из design_new.glade, подключает все функции к кнопкам.
 */
GraphEditorApp::GraphEditorApp() : ui{Gtk::Builder::create_from_file("design_new.glade")} {
    main_box = nullptr;
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

        this->tool_change_weight = Glib::RefPtr<Gtk::ToolButton>::cast_dynamic(
                ui->get_object("tool_change_weight")
        );
        this->tool_change_weight->signal_clicked().connect(
                sigc::mem_fun(*this, &GraphEditorApp::on_change_weight)
        );

        this->next_weight_label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("weight_label")
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
                sigc::bind(
                        sigc::mem_fun(*this, &GraphEditorApp::print_algorithm),
                        ""
                )
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

/**
 * @brief Обработка нового введённого веса, проверка на его корректность.
 *
 * @param new_weight строка, в которой записан новый вес.
 *
 * @return строка веса после обработки.
 *
 * Метод возвращает пустую строку, если новый вес некорректен - состоит не из цифр или является пустой строкой.
 * В случае успешной проверки на корректность возвращает первоначальную строку, приведённую к типу std::string.
 */
std::string GraphEditorApp::handling_new_weight(Glib::ustring new_weight) {
    if (new_weight.empty()) {
        return "";
    }

    bool is_negative = false;
    if (new_weight[0] == '-') {
        is_negative = true;
        new_weight.erase(0, 1);
    }

    bool is_number = true;
    for (auto i: new_weight) {
        if (i <= 47 or i >= 58) {
            is_number = false;
            break;
        }
    }

    if (!is_number) {
        return "";
    }
    if (is_negative) {
        return '-' + std::string(new_weight);
    }
    return std::string(new_weight);
}

/**
 * @brief Метод вызова смены веса.
 *
 * Метод подключен к кнопке смены веса, вызывается при ее нажатии
 */
void GraphEditorApp::on_change_weight() {
    WeightEntryDialog dialog(this->getCenterOfWindow().first, this->getCenterOfWindow().second);
    int result = dialog.run();
    if (result == Gtk::RESPONSE_OK) {
        std::cout << "Entered text: " << dialog.get_text() << std::endl;
    } else {
        std::cout << "Canceled" << std::endl;
        return;
    }

    std::string new_weight = this->handling_new_weight(dialog.get_text());
    if (!new_weight.empty()) {
        Graph::getInstance()->nextWeight = std::stoi(new_weight);
        this->next_weight_label->set_label(new_weight);
        std::cout << new_weight << '\n';
    } else {
        this->run_error_dialog("Entered wrong weight");
        return;
    }
}

/**
 * @brief Метод печати данных о графе.
 *
 * Метод подключен к кнопке печати графа (Print Graph / Close printout), вызывается при ее нажатии.
 * Если распечатка графа неактивна (имя кнопки - Print Graph), то метод показывает ярлык с основными данными графа (матрица и список смежности).
 * Если распечатка графа активна (имя кнопки - Close printout), то метод скрывает ярлык с данными графа
 */
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
/**
 * @brief Метод вызова алгоритма и его печать на ярлыке алгоритма.
 *
 * @param test_input альтернативный ввод запрашиваемых вершин для алгоритмов. Используется для тестирования метода,
 * поэтому по умолчанию этот параметр равен пустой строке ("").
 *
 * Метод подключен к кнопке запуска алгоритма (Run algorithm / Close algorithm), вызывается при ее нажатии.
 * Если распечатка алгоритма неактивна (имя кнопки - Run algorithm), то метод запускает выбранный алгоритм и показывает ярлык с текстом вывода алгоритма.
 * Если распечатка алгоритма активна (имя кнопки - Close algorithm), то метод скрывает ярлык с текстом вывода алгоритма.
 */
void GraphEditorApp::print_algorithm(const std::string &test_input) {
    if (this->run_algorithm_button->get_label() == "Run algorithm") {
        if (this->choose_algorithm_cb->get_active_id() == "Bellman-Ford"
            or this->choose_algorithm_cb->get_active_id() == "Dijkstra") {
            char first_vertex;
            char last_vertex;
            if (!test_input.empty()) {
                first_vertex = test_input[0];
                last_vertex = test_input[1];
            } else {
                PathEntryDialog dialog(this->getCenterOfWindow().first, this->getCenterOfWindow().second);
                int result = dialog.run();
                if (result == Gtk::RESPONSE_OK) {
                    printf("Entered text: %s %s\n", dialog.get_vertex_from().c_str(), dialog.get_vertex_to().c_str());
                } else {
                    printf("Canceled\n");
                    return;
                }
                first_vertex = (char) dialog.get_vertex_from()[0];
                last_vertex = (char) dialog.get_vertex_to()[0];
            }
            char max_vertex = Graph::getInstance()->TITLES[Graph::getInstance()->ID_NEXT_TITLE - 1];
            if (first_vertex < 'A' or first_vertex > max_vertex or last_vertex < 'A' or last_vertex > max_vertex) {
                std::cout << "Entered wrong vertices!\n";
                if (test_input.empty()) this->run_error_dialog("Entered wrong vertices");
                return;
            }
            Graph::getInstance()->runAlgorithm(this->choose_algorithm_cb->get_active_id(),
                                               first_vertex, last_vertex);
            std::cout << "Ran algorithm " << this->choose_algorithm_cb->get_active_id() << ", ";
            std::cout << first_vertex << "->" << last_vertex << ";\n";
        } else if (this->choose_algorithm_cb->get_active_id() == "DFS"
                   or this->choose_algorithm_cb->get_active_id() == "BFS") {
            char start_vertex;
            if (!test_input.empty()) {
                start_vertex = test_input[0];
            } else {
                VertexEntryDialog dialog(this->getCenterOfWindow().first, this->getCenterOfWindow().second);
                int result = dialog.run();
                if (result == Gtk::RESPONSE_OK) {
                    std::cout << "Entered text: " << dialog.get_text() << std::endl;
                } else {
                    std::cout << "Canceled" << std::endl;
                    return;
                }
                start_vertex = (char) dialog.get_text()[0];
            }

            char max_vertex = Graph::getInstance()->TITLES[Graph::getInstance()->ID_NEXT_TITLE - 1];
            if (start_vertex < 'A' or start_vertex > max_vertex) {
                std::cout << "Entered wrong vertices!\n";
                if (test_input.empty()) this->run_error_dialog("Entered wrong vertices");
                return;
            }

            Graph::getInstance()->runAlgorithm(this->choose_algorithm_cb->get_active_id(), start_vertex);
            std::cout << "Ran algorithm " << this->choose_algorithm_cb->get_active_id();
            std::cout << " with start vertex " << start_vertex << ";\n";
        } else {
            Graph::getInstance()->runAlgorithm(this->choose_algorithm_cb->get_active_id());
            std::cout << "Ran algorithm " << this->choose_algorithm_cb->get_active_id() << ";\n";
        }
        this->printed_algorithm_label->set_text(Graph::getInstance()->getPrintoutAlgorithm());
        this->printed_algorithm_label->show();
        this->run_algorithm_button->set_label("Close algorithm");

        // Добавление вызова clearScreenAndRestoreGraph()
        Canvas::getInstance()->clearScreenAndRestoreGraph();
    } else {
        this->printed_algorithm_label->hide();
        this->printed_algorithm_label->hide();
        this->run_algorithm_button->set_label("Run algorithm");
    }
}

/**
 * @brief Метод получения координат центра окна программы.
 *
 * @return std::pair <int, int>, в первой переменной которой записана координата оX, во второй - координата оY
 *
 * Метод возвращает координаты центра окна программы.
 * Используется для вызова всех диалогов, так как они запускаются в центре окна программы
 */
std::pair<int, int> GraphEditorApp::getCenterOfWindow() {
    int corner_x, corner_y, size_x, size_y;
    this->get_position(corner_x, corner_y);
    this->get_size(size_x, size_y);
    return {corner_x + size_x / 2, corner_y + size_y / 2};
}

/**
 * @brief Метод запуска ошибки с сообщением.
 *
 * @param message строка, которая будет напечатана в сообщении ошибки
 *
 * Метод вызывается из мест, когда нужно оповестить пользователя о неверно введённых данных или неправильной работе программы.
 * Диалог ошибки содержит поле с сообщением, в которое записывается строка message
 */
void GraphEditorApp::run_error_dialog(const std::string& message) {
    Gtk::MessageDialog error_dialog(
            message,
            false, Gtk::MESSAGE_ERROR,
            Gtk::BUTTONS_OK, true);

    int size_x, size_y;
    error_dialog.get_size(size_x, size_y);
    error_dialog.move(this->getCenterOfWindow().first - size_x / 2,
                      this->getCenterOfWindow().second - size_y / 2);
    error_dialog.run();
}

/**
 * @brief Метод для установки нового алгоритма.
 *
 * @param new_algorithm строка, в которой записан требуемый алгоритм
 *
 * Метод для тестов. Устанавливает новое выбранное значение в choose_algorithm_cb.
 * Причём параметр new_algorithm должен точно совпадать с одним из существующих в программе алгоритмов
 */
void GraphEditorApp::setAlgorithmName(const std::string &new_algorithm) {
    std::set<std::string> allowed_algorithms = {"BFS", "DFS", "Bellman-Ford", "Dijkstra", "Prim", "Kruskal"};
    if (allowed_algorithms.find(new_algorithm) != allowed_algorithms.end()) {
        this->choose_algorithm_cb->set_active_id(new_algorithm);
    }
}

/**
 * @brief Метод получения названия активного алгоритма.
 *
 * @return строка, в которой записан активный алгоритм
 *
 * Метод для тестов. Возвращает название активного алгоритма из choose_algorithm_cb
 */
std::string GraphEditorApp::getAlgorithmName() const {
    return this->choose_algorithm_cb->get_active_id();
}
