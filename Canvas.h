//
// Created by Dmitriy on 02.04.2024.
//

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


class Canvas : public Gtk::DrawingArea {
private:
    static Canvas* instance;
    Canvas();

    int state;
    Color color;
    Cairo::RefPtr <Cairo::Surface> buffer;
    Cairo::RefPtr <Cairo::Surface> temp_buffer;
    double buffer_width, buffer_height;
    double start_x, start_y;
    bool need_fix_temp_buffer;

    Gtk::ColorChooserDialog *color_chooser_dialog;
public:
    static Canvas* getInstance();

    static const int DEFAULT = 1 << 0;//состояния программы
    static const int DRAWING = 1 << 1;
    static const int VERTEX = 1 << 2;
    static const int EDGE = 1 << 3;

    void drawing_arrow(const double x1, const double y1, const double x2, const double y2);

    void choose_color_response(int response_id);

    void choose_color();

    bool on_mouse_press(GdkEventButton *event);

    bool on_mouse_move(GdkEventMotion *event);

    bool on_mouse_release(GdkEventButton *event);

    Cairo::RefPtr <Cairo::Context> get_context(Cairo::RefPtr <Cairo::Surface> &surface, bool need_clear = false);

    void drawing_vertex(double x, double y, char name);

    void drawing(double x, double y);

    bool on_draw(const Cairo::RefPtr <Cairo::Context> &cr) override;

    void change_tool(int tool);

    //useless
    void animate_bfs(const std::string &bfs_result);

    //useless
    void visualize_vertex(char vertex, Color color);

    void outline_vertex(char vertex, Color outline_color);

    void fill_vertex(char vertex, Color fill_color);

    void redraw_edge(char vertex1, char vertex2, Color outline_color);

    void draw_colored_arrow(double start_x, double start_y, double end_x, double end_y, Color outline_color);

    void clear_screen();

    void clearScreenAndRestoreGraph();

    void redrawGraph();

    void clear_temp_buffer();

    void update_main_buffer();

    void redraw_all();

    void save_current_state();
};


#endif //UNTITLED_CANVAS_H
