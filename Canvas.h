/**
 * @file Canvas.h
 * @brief Загаловочный файл для  Canvas.cpp. Содержит функции, реализующие процесс рисования графа и анимации алгоритмов.
 */
#ifndef UNTITLED_CANVAS_H
#define UNTITLED_CANVAS_H
#include <cairo.h>
#include "gtkmm.h"
#include "cairomm/context.h"
#include <cstdlib>
#include <string>
#include <cmath>
#include <map>
#include <unistd.h>
#include <queue>
#include <iostream>
#include "Graph.h"
#include "Color.h"
#include "VertexEntryDialog.h"

/**
 * @brief Класс, реализующий графическое представление графа и анимации алгоритмов.
 */
class Canvas : public Gtk::DrawingArea {
private:
    static Canvas* instance;
    Canvas();

    int state;
    Color color;
    double buffer_width, buffer_height;
    double start_x, start_y;
    bool need_fix_temp_buffer;
    std::pair<char, char> next_edge_to_append;

    Gtk::ColorChooserDialog *color_chooser_dialog;
    friend class GraphTestSuite;
    friend class GraphTestAccessor;

public:
    static Canvas* getInstance();

    static const int DEFAULT;
    static const int DRAWING;
    static const int VERTEX;
    static const int EDGE;

    /**
     * @brief Получает текущее состояние холста.
     * @return Текущее состояние холста.
     */
    int getState() const;

    /**
     * @brief Рисует стрелку вместе с ее весом на холсте.
     * @param x1 Начальная x-координата.
     * @param y1 Начальная y-координата.
     * @param x2 Конечная x-координата.
     * @param y2 Конечная y-координата.
     */
    void drawing_arrow(const double x1, const double y1, const double x2, const double y2);

    /**
     * @brief Обрабатывает выбор цвета.
     * @param response_id Идентификатор ответа выбора цвета.
     */
    void choose_color_response(int response_id);

    /**
     * @brief Открывает диалог выбора цвета.
     */
    void choose_color();

    /**
     * @brief Обрабатывает нажатие кнопки мыши.
     * @param event Событие нажатия кнопки мыши.
     * @return true, если событие обработано.
     */
    bool on_mouse_press(GdkEventButton *event);

    /**
     * @brief Обрабатывает движение мыши.
     * @param event Событие движения мыши.
     * @return true, если событие обработано.
     */
    bool on_mouse_move(GdkEventMotion *event);

    /**
     * @brief Обрабатывает отпускание кнопки мыши.
     * @param event Событие отпускания кнопки мыши.
     * @return true, если событие обработано.
     */
    bool on_mouse_release(GdkEventButton *event);

    /**
     * @brief Получает контекст рисования для указанной поверхности.
     * @param surface Поверхность для получения контекста.
     * @param need_clear Нужно ли очищать поверхность перед использованием.
     * @return Контекст рисования.
     */
    Cairo::RefPtr <Cairo::Context> get_context(Cairo::RefPtr <Cairo::Surface> &surface, bool need_clear = false);

    /**
     * @brief Рисует вершину на холсте.
     * @param x X-координата вершины.
     * @param y Y-координата вершины.
     * @param name Имя вершины.
     */
    void drawing_vertex(double x, double y, char name);

    /**
     * @brief Рисует элементы на холсте, используя необходимые функции рисования.
     * @param x X-координата.
     * @param y Y-координата.
     */
    void drawing(double x, double y);

    /**
     * @brief Отрисовывает текущее состояние холста.
     * @param cr Контекст рисования.
     * @return true, если отрисовка успешна.
     */
    bool on_draw(const Cairo::RefPtr <Cairo::Context> &cr) override;

    /**
     * @brief Меняет инструмент рисования.
     * @param tool Новый инструмент.
     */
    void change_tool(int tool);

    /**
     * @brief Обводит вершину указанным цветом. Необходима для анимации алгоритмов.
     * @param vertex Вершина для обводки.
     * @param outline_color Цвет обводки.
     */
    void outline_vertex(char vertex, Color outline_color);

    /**
     * @brief Перерисовывает ребро между двумя вершинами указанным цветом. Необходима для анимации алгоритмов.
     * @param vertex1 Первая вершина.
     * @param vertex2 Вторая вершина.
     * @param outline_color Цвет обводки.
     */
    void redraw_edge(char vertex1, char vertex2, Color outline_color);

    /**
     * @brief Рисует цветную стрелку на холсте. Необходима для анимации алгоритмов.
     * @param start_x Начальная x-координата.
     * @param start_y Начальная y-координата.
     * @param end_x Конечная x-координата.
     * @param end_y Конечная y-координата.
     * @param outline_color Цвет стрелки.
     */
    void draw_colored_arrow(double start_x, double start_y, double end_x, double end_y, Color outline_color);

    /**
     * @brief Очищает экран. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
     */
    void clear_screen();

    /**
     * @brief Очищает экран и восстанавливает граф. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
     */
    void clearScreenAndRestoreGraph();

    /**
     * @brief Перерисовывает граф. Необходима при выполнении анимации нового алгоритма после выполнения другого алгоритма.
     */
    void redrawGraph();

    /**
     * @brief Очищает временный буфер, заполняя его белым цветом.
     */
    void clear_temp_buffer();

    /**
     * @brief Обновляет основной буфер.
     */
    void update_main_buffer();


    Cairo::RefPtr <Cairo::Surface> temp_buffer;
    Cairo::RefPtr <Cairo::Surface> buffer;

    std::tuple<double, double, double, double>
    calculateArrowCoordinates(double start_x, double start_y, double end_x, double end_y, double vertex_radius);
};


#endif //UNTITLED_CANVAS_H
