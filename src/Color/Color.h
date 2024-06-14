//
// Created by Dmitriy on 01.05.2024.
//

#ifndef GRAPHEDITOR_COLOR_H
#define GRAPHEDITOR_COLOR_H

/**
 * @brief Класс, реализующий цвета (RGBa).
 */
class Color {
public:
    double r, g, b, a; ///< Значения каналов цвета

    /**
     * @brief Конструктор.
     */
    Color(double r, double g, double b, double a);
};


#endif //GRAPHEDITOR_COLOR_H
