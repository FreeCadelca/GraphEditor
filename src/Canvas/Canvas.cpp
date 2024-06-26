/**
 * @file Canvas.cpp
 * @brief Файл реализации для класса Canvas. Содержит функции, реализующие процесс рисования графа и анимации алгоритмов.
 */
#include "Canvas.h"

// Инициализация статических членов
const int Canvas::DEFAULT = 1 << 0;
const int Canvas::DRAWING = 1 << 1;
const int Canvas::VERTEX = 1 << 2;
const int Canvas::EDGE = 1 << 3;
//std::map<char, std::vector<char>> adjacent; //словарь смежности
//
//std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//названия вершин
//int ID_NEXT_TITLE = 0;//номер следующей вершины для выбора
// !Legacy!
Canvas *Canvas::instance = nullptr;

/**
 * @brief Получает экземпляр синглтона класса Canvas.
 * @return Указатель на экземпляр Canvas.
 */
Canvas *Canvas::getInstance() {
    if (instance == nullptr) {
        instance = new Canvas();
    }
    return instance;
}

/**
 * @brief Конструктор для класса Canvas, реализующий графическое представление графа и анимации алгоритмов.
 */
Canvas::Canvas() : state(DEFAULT | VERTEX), color(0, 0, 0, 1), buffer_width(1920), buffer_height(1080),
                   need_fix_temp_buffer(false) {
    //конструктор. В него пришлось передавать лейбл распечатки, потому что по-другому не получилось.
    // Остальные параметры понятны по названию
    this->signal_draw().connect(sigc::mem_fun(*this, &Canvas::on_draw));
    this->set_vexpand(true);
    this->add_events(Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    this->signal_button_press_event().connect(sigc::mem_fun(*this, &Canvas::on_mouse_press));
    this->signal_motion_notify_event().connect(sigc::mem_fun(*this, &Canvas::on_mouse_move));
    this->signal_button_release_event().connect(sigc::mem_fun(*this, &Canvas::on_mouse_release));
    this->buffer = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, buffer_width, buffer_width);
    this->temp_buffer = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, buffer_width, buffer_width);
    auto context = Cairo::Context::create(this->buffer);//рисование холста (заливка белым)
    context->set_source_rgb(1, 1, 1);
    context->rectangle(0, 0, buffer_width, buffer_height);
    context->fill();
    context->stroke();

    this->color_chooser_dialog = new Gtk::ColorChooserDialog;//настройка диалога выбора цвета
    this->color_chooser_dialog->set_modal(true);
    this->color_chooser_dialog->signal_response().connect(sigc::mem_fun(*this, &Canvas::choose_color_response));
}


/**
 * @brief Получает текущее состояние холста.
 * @return Текущее состояние холста.
 */
int Canvas::getState() const {
    return state;
}

/**
 * @brief Обрабатывает выбор цвета.
 * @param response_id Идентификатор ответа выбора цвета.
 */
void Canvas::choose_color_response(int response_id) {//настройка цвета, выбранного через диалог
    if (response_id == Gtk::RESPONSE_OK) {
        auto res = this->color_chooser_dialog->get_rgba();
        this->color = {res.get_red(), res.get_green(), res.get_blue(), res.get_alpha()};

    }
    this->color_chooser_dialog->hide();
}

/**
 * @brief Открывает диалог выбора цвета.
 */
void Canvas::choose_color() {//запуск диалога выбора цвета
    this->color_chooser_dialog->run();
}

/**
 * @brief Обрабатывает нажатие кнопки мыши.
 * @param event Событие нажатия кнопки мыши.
 * @return true, если событие обработано.
 */
bool Canvas::on_mouse_press(GdkEventButton *event) {//прописывание функционала при нажатой мыши
    this->state &= ~DEFAULT;
    this->state |= DRAWING;
    auto context = this->get_context(temp_buffer, true);
    this->start_x = event->x;
    this->start_y = event->y;
    this->drawing(event->x, event->y);
    return true;
}

/**
 * @brief Обрабатывает движение мыши.
 * @param event Событие движения мыши.
 * @return true, если событие обработано.
 */
bool Canvas::on_mouse_move(GdkEventMotion *event) {//прописывание функционала при двигающейся мыши
    this->state &= ~DEFAULT;//замена состояния
    this->state |= DRAWING;
    if ((this->state & VERTEX) == 0) {
        //если мы рисуем вершину, то нарисовав ее в начале, нам не надо дальше рисовать их, пока зажата мышь
        this->drawing(event->x, event->y);
        return true;
    }
    return true;
}

/**
 * @brief Обрабатывает отпускание кнопки мыши.
 * @param event Событие отпускания кнопки мыши.
 * @return true, если событие обработано.
 */
bool Canvas::on_mouse_release(GdkEventButton *event) {//прописывание функционала при отпускании мыши
    this->state &= ~DRAWING;//замена состояния
    this->state |= DEFAULT;
    this->drawing(event->x, event->y);
    return true;
}

/**
 * @brief Получает контекст рисования для указанной поверхности.
 * @param surface Поверхность для получения контекста.
 * @param need_clear Нужно ли очищать поверхность перед использованием.
 * @return Контекст рисования.
 */
Cairo::RefPtr<Cairo::Context> Canvas::get_context(Cairo::RefPtr<Cairo::Surface> &surface, bool need_clear) {
    //функция "вытаскивания" контекста (раньше мы называли его "cr") для рисования
    auto context = Cairo::Context::create(surface);
    if (need_clear) {//очищаем контекст при надобности
        context->set_source_rgba(0, 0, 0, 0);
        context->set_operator(Cairo::OPERATOR_CLEAR);
        context->rectangle(0, 0, buffer_width, buffer_height);
        context->paint_with_alpha(1);
        context->stroke();
    }
    context->set_source_rgba(this->color.r, this->color.g, this->color.b, this->color.a);
    //настраиваем выбранный ранее цвет
    context->set_operator(Cairo::OPERATOR_OVER);
    return context;
}

/**
 * @brief Рисует вершину на холсте.
 * @param x X-координата вершины.
 * @param y Y-координата вершины.
 * @param name Имя вершины.
 */
void Canvas::drawing_vertex(double x, double y, char name) {
    //функция рисования вершин (с белой подложкой для "перекрытия" наложенных на вершину рёбер)
    std::string s;//перевод названия в тип string для "CLang-Tidy, как просит Clion)
    s.push_back(name);

    auto context = this->get_context(temp_buffer);

    context->set_source_rgba(1, 1, 1, 1); // рисование подложки
    context->arc(x, y, 20, 0, M_PI * 2);
    context->fill();

    context = this->get_context(temp_buffer);

    context->arc(x, y, 20, 0, M_PI * 2);
    context->set_line_width(6);
    context->stroke();

    cairo_text_extents_t te; // рисование названия вершины
    context->select_font_face("Calibri", static_cast<Cairo::FontSlant>(CAIRO_FONT_SLANT_NORMAL),
                              static_cast<Cairo::FontWeight>(CAIRO_FONT_WEIGHT_BOLD));
    context->set_font_size(20);
    context->get_text_extents(s, te);
    context->move_to(x - te.width / 2, y + te.height / 2);
    context->show_text(s);

    context->stroke();
}

/**
 * @brief Вычисляет скорректированные координаты для рисования стрелки.
 * @param start_x Начальная x-координата.
 * @param start_y Начальная y-координата.
 * @param end_x Конечная x-координата.
 * @param end_y Конечная y-координата.
 * @param vertex_radius Радиус вершины.
 * @return Кортеж, содержащий скорректированные координаты начала и конца.
 */
std::tuple<double, double, double, double>
Canvas::calculateArrowCoordinates(double start_x, double start_y, double end_x, double end_y, double vertex_radius) {
    // Нормализация вектора направления ребра
    double length = std::sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
    double dir_x = (end_x - start_x) / length;
    double dir_y = (end_y - start_y) / length;

    // Коррекция координат начала и конца стрелки
    start_x += dir_x * vertex_radius;
    start_y += dir_y * vertex_radius;
    end_x -= dir_x * vertex_radius;
    end_y -= dir_y * vertex_radius;

    // Смещение начала и конца стрелки в сторону от вершины
    double offset_x = dir_y * vertex_radius * 0.25;
    double offset_y = -dir_x * vertex_radius * 0.25;
    start_x += offset_x;
    start_y += offset_y;
    end_x += offset_x;
    end_y += offset_y;

    return std::make_tuple(start_x, start_y, end_x, end_y);
}

/**
 * @brief Рисует стрелку вместе с ее весом на холсте.
 * @param x1 Начальная x-координата.
 * @param y1 Начальная y-координата.
 * @param x2 Конечная x-координата.
 * @param y2 Конечная y-координата.
 */
void Canvas::drawing_arrow(double start_x, double start_y, double end_x, double end_y) {
    this->need_fix_temp_buffer = true;
    double length = std::sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
    double dir_x = (end_x - start_x) / length;
    double dir_y = (end_y - start_y) / length;
    // Определение, к каким вершинам относятся координаты начала и конца ребра
    char from_vertex = '-';
    char to_vertex = '-';
    for (const auto &vertex_pair: Graph::getInstance()->coords) {
        if (std::abs(vertex_pair.second.first - start_x) <= 20 && std::abs(vertex_pair.second.second - start_y) <= 20) {
            from_vertex = vertex_pair.first;
        }
        if (std::abs(vertex_pair.second.first - end_x) <= 20 && std::abs(vertex_pair.second.second - end_y) <= 20) {
            to_vertex = vertex_pair.first;
        }
    }

    // Поиск веса ребра между вершинами
    int weight = Graph::getInstance()->adjacent_matrix[from_vertex - 'A'][to_vertex - 'A'];

    auto context = this->get_context(temp_buffer, false);
    context->set_line_width(2);

    // Расстояние от начала и конца стрелки до центра вершины
    double vertex_radius = 20;

    // Вычисляем новые координаты начала и конца стрелки
    std::tie(start_x, start_y, end_x, end_y) = calculateArrowCoordinates(start_x, start_y, end_x, end_y, vertex_radius);

    // Рисование стрелки
    context->move_to(start_x, start_y);
    context->line_to(end_x, end_y);
    context->stroke();

    // Переносим начало координат в конечную точку
    context->translate(end_x, end_y);
    double angle = std::atan2(dir_y, dir_x);
    context->rotate(angle);
    double arrow_length = 20;
    double arrow_width = 10;
    context->move_to(0, 0);
    context->line_to(-arrow_length, arrow_width / 2);
    context->line_to(-arrow_length, -arrow_width / 2);
    context->close_path();
    context->fill();

    // Рисуем вес ребра в середине стрелки
    double mid_x = (start_x + end_x) / 2;
    double mid_y = (start_y + end_y) / 2;

    context->save();
    context->rotate(-angle);
    context->translate(-end_x, -end_y);

    context->set_font_size(20);
    context->move_to(mid_x, mid_y);
    context->show_text(std::to_string(weight));
    context->restore();

    this->queue_draw();
}

/**
 * @brief Рисует элементы на холсте, используя необходимые функции рисования.
 * @param x X-координата.
 * @param y Y-координата.
 */
void Canvas::drawing(double x, double y) {
    if (this->state & DRAWING) {
        this->need_fix_temp_buffer = true;
        if (this->state & VERTEX) {
            // Рисование вершины
            bool can_draw = true;
            for (auto i: Graph::getInstance()->coords) {
                if (abs(i.second.first - x) <= 80 and abs(i.second.second - y) <= 80) {
                    can_draw = false;
                    break;
                }
            }

            if (can_draw and Graph::getInstance()->ID_NEXT_TITLE != Graph::getInstance()->TITLES.size()) {
                char next_char = Graph::getInstance()->TITLES[Graph::getInstance()->ID_NEXT_TITLE];
                Graph::getInstance()->addVertex(x, y);
                drawing_vertex(x, y, next_char);

                // Обновляем основной буфер и инициируем перерисовку
                update_main_buffer();
                this->queue_draw();
            }
        }
        if (this->state & EDGE) {
            // Рисование ребра
            char sticking_from_vertex = '-';
            for (auto i: Graph::getInstance()->coords) {
                if (abs(i.second.first - start_x) <= 40 and abs(i.second.second - start_y) <= 40) {
                    start_x = i.second.first;
                    start_y = i.second.second;
                    sticking_from_vertex = i.first;
                    break;
                }
            }
            if (sticking_from_vertex != '-') {
                char sticking_to_vertex = '-';
                for (auto i: Graph::getInstance()->coords) {
                    if (abs(i.second.first - x) <= 40 and abs(i.second.second - y) <= 40) {
                        x = i.second.first;
                        y = i.second.second;
                        sticking_to_vertex = i.first;
                        break;
                    }
                }
                if (sticking_to_vertex != sticking_from_vertex and sticking_to_vertex != '-') {
                    bool edge_exist = false;
                    for (auto i: Graph::getInstance()->adjacent_list[sticking_from_vertex]) {
                        if (sticking_to_vertex == i) {
                            edge_exist = true;
                            break;
                        }
                    }
                    if (!edge_exist) {
                        Graph::getInstance()->addEdge(sticking_from_vertex, sticking_to_vertex);
                        drawing_arrow(start_x, start_y, x, y);

                        // Обновляем основной буфер и инициируем перерисовку
                        update_main_buffer();
                        this->queue_draw();
                    }
                }
            }
        }
    }
}

/**
 * @brief Отрисовывает текущее состояние холста.
 * @param cr Контекст рисования.
 * @return true, если отрисовка успешна.
 */
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->set_source(this->buffer, 0, 0);
    cr->paint();
    if (this->state & DRAWING) {
        cr->set_source(this->temp_buffer, 0, 0);
        cr->paint();
    }
    return true;
}

/**
 * @brief Меняет инструмент рисования.
 * @param tool Новый инструмент.
 */
void Canvas::change_tool(int tool) {//функция смены инструмента
    this->state = Canvas::DEFAULT | tool;
    if (tool == VERTEX) {

    }
}

/**
 * @brief Обводит вершину указанным цветом. Необходима для анимации алгоритмов.
 * @param vertex Вершина для обводки.
 * @param outline_color Цвет обводки.
 */
void Canvas::outline_vertex(char vertex, Color outline_color) {
    // Получаем координаты вершины
    double x = Graph::getInstance()->coords[vertex].first;
    double y = Graph::getInstance()->coords[vertex].second;

    // Получаем контекст рисования для временного буфера
    auto context = this->get_context(temp_buffer);

    // Начинаем рисование окружности с обводкой
    context->set_source_rgba(outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    context->arc(x, y, 27, 0, M_PI * 2);
    context->set_line_width(8); // Устанавливаем толщину обводки
    context->stroke();

    // Применяем изменения из temp_buffer в основной буфер buffer
    auto main_context = this->get_context(buffer);
    main_context->set_source(temp_buffer, 0, 0);
    main_context->paint();

    // Перерисовываем холст, чтобы отобразить изменения
    this->queue_draw();
}

/**
 * @brief Рисует цветную стрелку на холсте. Необходима для анимации алгоритмов.
 * @param start_x Начальная x-координата.
 * @param start_y Начальная y-координата.
 * @param end_x Конечная x-координата.
 * @param end_y Конечная y-координата.
 * @param outline_color Цвет стрелки.
 */
void Canvas::draw_colored_arrow(double start_x, double start_y, double end_x, double end_y, Color outline_color) {
    auto context = this->get_context(temp_buffer);
    context->set_source_rgba(outline_color.r, outline_color.g, outline_color.b, outline_color.a);
    context->set_line_width(2);

    // Расстояние от начала и конца стрелки до центра вершины
    double vertex_radius = 20;

    // Нормализация вектора направления ребра
    double length = sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
    double dir_x = (end_x - start_x) / length;
    double dir_y = (end_y - start_y) / length;

    // Коррекция координат начала и конца стрелки
    start_x += dir_x * vertex_radius;
    start_y += dir_y * vertex_radius;
    end_x -= dir_x * vertex_radius;
    end_y -= dir_y * vertex_radius;

    // Смещение начала и конца стрелки в сторону от вершины
    double offset_x = dir_y * vertex_radius * 0.25;
    double offset_y = -dir_x * vertex_radius * 0.25;
    start_x += offset_x;
    start_y += offset_y;
    end_x += offset_x;
    end_y += offset_y;

    // Рисование стрелки
    context->move_to(start_x, start_y);
    context->line_to(end_x, end_y);
    context->stroke();

    // Рисование наконечника стрелки
    context->save();
    context->translate(end_x, end_y);
    double angle = atan2(dir_y, dir_x);
    context->rotate(angle);
    double arrow_length = 20;
    double arrow_width = 10;
    context->move_to(0, 0);
    context->line_to(-arrow_length, arrow_width / 2);
    context->line_to(-arrow_length, -arrow_width / 2);
    context->close_path();
    context->fill();
    context->restore();

    // Применяем изменения из temp_buffer в основной буфер buffer
    auto main_context = this->get_context(buffer);
    main_context->set_source(temp_buffer, 0, 0);
    main_context->paint();

    // Перерисовываем холст, чтобы отобразить изменения
    this->queue_draw();
}

/**
 * @brief Перерисовывает ребро между двумя вершинами указанным цветом. Необходима для анимации алгоритмов.
 * @param vertex1 Первая вершина.
 * @param vertex2 Вторая вершина.
 * @param outline_color Цвет обводки.
 */
void Canvas::redraw_edge(char vertex1, char vertex2, Color outline_color) {
    // Получаем координаты вершин
    double start_x = Graph::getInstance()->coords[vertex1].first;
    double start_y = Graph::getInstance()->coords[vertex1].second;
    double end_x = Graph::getInstance()->coords[vertex2].first;
    double end_y = Graph::getInstance()->coords[vertex2].second;

    // Вызываем функцию рисования стрелки указанным цветом
    draw_colored_arrow(start_x, start_y, end_x, end_y, outline_color);
}

/**
 * @brief Очищает экран. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
 */
void Canvas::clear_screen() {
    auto context = this->get_context(buffer, true);
    context->set_source_rgb(1, 1, 1); // белый цвет
    context->rectangle(0, 0, buffer_width, buffer_height);
    context->fill();
    context->stroke();
    this->queue_draw();

}

/**
 * @brief Очищает экран и восстанавливает граф. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
 */
void Canvas::clearScreenAndRestoreGraph() {
    // Шаг 1: Сохраняем текущее состояние матрицы смежности
    std::vector<std::vector<int>> savedAdjacentMatrix = Graph::getInstance()->adjacent_matrix;

    // Шаг 2: Очищаем экран
    clear_screen(); // Предположим, что у вас есть метод clear_screen() в классе Canvas
    buffer = temp_buffer;
    // Шаг 3: Восстанавливаем граф на основе сохраненной матрицы смежности
    Graph::getInstance()->adjacent_matrix = savedAdjacentMatrix;

    // Шаг 4: Перерисовываем граф на холсте
    redrawGraph();
}

/**
 * @brief Перерисовывает граф. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
 */
void Canvas::redrawGraph() {
    if (!Graph::getInstance()->coords.empty()) {
        // Очищаем временный буфер
        clear_temp_buffer();

        // Устанавливаем временное состояние DRAWING для перерисовки рёбер
        this->state |= DRAWING;

        // Перерисовываем рёбра графа на холсте
        for (const auto &edge_pair: Graph::getInstance()->adjacent_list) {
            char from_vertex = edge_pair.first;
            const auto &adjacent_vertices = edge_pair.second;
            for (char to_vertex: adjacent_vertices) {
                double start_x = Graph::getInstance()->coords[from_vertex].first;
                double start_y = Graph::getInstance()->coords[from_vertex].second;
                double end_x = Graph::getInstance()->coords[to_vertex].first;
                double end_y = Graph::getInstance()->coords[to_vertex].second;
                drawing_arrow(start_x, start_y, end_x, end_y);
            }
        }

        // Перерисовываем вершины графа на холсте
        for (const auto &vertex_pair: Graph::getInstance()->coords) {
            char vertex = vertex_pair.first;
            double x = vertex_pair.second.first;
            double y = vertex_pair.second.second;
            drawing_vertex(x, y, vertex);
        }

        // Обновляем основной буфер из временного буфера
        update_main_buffer();

        // Перерисовываем область экрана
        this->queue_draw();

        // Возвращаем исходное состояние
        this->state &= ~DRAWING;
    }
}


/**
 * @brief Очищает временный буфер, заполняя его белым цветом.
 */
void Canvas::clear_temp_buffer() {
    auto context = this->get_context(temp_buffer, true);
    context->set_source_rgb(1, 1, 1); // белый цвет
    context->rectangle(0, 0, buffer_width, buffer_height);
    context->fill();
    context->stroke();
}

/**
 * @brief Обновляет основной буфер.
 */
void Canvas::update_main_buffer() {
    auto context = this->get_context(buffer);
    context->set_source(this->temp_buffer, 0, 0);
    context->paint();
}
