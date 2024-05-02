//
// Created by Dmitriy on 20.04.2024.
//

#include "Graph.h"
#include <stack>
#include <functional>

Graph *Graph::instance = nullptr;

Graph::Graph() = default;

Graph *Graph::getInstance() {
    if (instance == nullptr) {
        instance = new Graph();
    }
    return instance;
}

void Graph::addVertex(double x, double y) {
    this->coords[TITLES[ID_NEXT_TITLE]] = {x, y};//добавляем новую вершину туда, куда нужно

    this->adjacent_list[TITLES[ID_NEXT_TITLE]] = {};

//    this->adjacent_matrix[TITLES[ID_NEXT_TITLE]] = std::map<char, int> ();
    if (ID_NEXT_TITLE == 0) {
        this->adjacent_matrix = std::vector<std::vector<int>>(1, std::vector<int>(1, 0));
    } else {
        adjacent_matrix.resize(
                adjacent_matrix.size() + 1,
                std::vector<int>(adjacent_matrix.size(), 0)
        );
        for (auto &i: adjacent_matrix) {
            i.resize(i.size() + 1, 0);
        }
    }
    ID_NEXT_TITLE++;
}

void Graph::addEdge(char v_from, char v_to) {
    this->adjacent_list[v_from].push_back(v_to);
// A = 65
    int index_v_from = (int) v_from - 65;
    int index_v_to = (int) v_to - 65;
    this->adjacent_matrix[index_v_from][index_v_to] = this->nextWeight;
}

std::string Graph::getPrintoutAdjList() {
    std::string output;
    int count_vertexes = (int) this->adjacent_list.size();
    for (auto i: this->adjacent_list) {
        char vertex = i.first;
        output.push_back(vertex);
        output += ":";
        for (auto edge: i.second) {
            output += " ";
            output.push_back(edge);
        }
        if (i.second.empty()) {
            output += " None";
        }
        output += ";\n";
    }
    return output;
}

//
std::string Graph::getPrintoutAdjMatrix() {
    int count_vertexes = (int) this->adjacent_list.size();
    std::string output = "    ";
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i];
        output += ' ';
    }
    output += '\n';
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i];
        output += ": ";
        for (auto j: this->adjacent_matrix[i]) {
            output += std::to_string(j) + " ";
        }
        output += '\n';
    }
    return output;
}

std::string Graph::getPrintoutAlgorithm() {
    return this->printoutAlgorithm;
}

void Graph::run_bfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (this->coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата BFS
    std::stringstream result;
    result << "Обход в ширину начиная с вершины " << start_vertex << ":\n";
    result << "Посещенные вершины: ";

    // Очередь для обхода графа в ширину
    std::queue<char> q;
    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Начинаем обход с заданной начальной вершины
    q.push(start_vertex);
    visited.insert(start_vertex);

    // Отрисовка первой вершины
    Canvas::getInstance()->outline_vertex(start_vertex, Color(0.6, 1, 0.6, 1));
    usleep(400000); // Добавим небольшую задержку перед началом анимации

    // Задержка между отрисовками вершин (в миллисекундах)
    int delay_ms = 400;

    // Обработка вершин с задержкой
    while (!q.empty()) {
        char current_vertex = q.front();
        q.pop();

        // Добавляем текущую вершину в результат обхода
        result << current_vertex << " ";

        // Получаем соседей текущей вершины из таблицы смежности
        auto neighbors = this->adjacent_list[current_vertex];
        // Для каждого соседа, если он еще не посещен, добавляем его в очередь и отмечаем как посещенный
        for (char neighbor: neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);

                // Создаем таймер для отложенной отрисовки вершины
                Glib::signal_timeout().connect([=]() {
                    Canvas::getInstance()->outline_vertex(neighbor, Color(0.6, 1, 0.6, 1));
                    return false; // Отключаем таймер после одного выполнения
                }, delay_ms);

                // Увеличиваем задержку перед следующей отрисовкой
                delay_ms += 400;
            }
        }
    }

    // Выводим результат BFS в программное окно
    this->printoutAlgorithm = result.str();
}

#include <functional>

#include <gtkmm/main.h>
#include <glibmm/main.h>
#include <iostream>
#include <sstream>
#include <stack>
#include <set>
#include <queue>
#include <functional>
#include <unistd.h> // Для usleep

void Graph::run_dfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата DFS
    std::stringstream result;
    result << "Обход в глубину начиная с вершины " << start_vertex << ":\n";
    result << "Посещенные вершины: ";

    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Стек для обхода графа в глубину
    std::stack<char> stack;

    // Создаем список вершин для посещения
    std::vector<char> visit_order;

    // Функция для обхода графа в глубину и формирования списка вершин для посещения
    std::function<void(char)> dfs_visit = [&](char vertex) {
        visited.insert(vertex);
        visit_order.push_back(vertex);
        auto neighbors = this->adjacent_list[vertex];
        for (char neighbor: neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                stack.push(neighbor);
                dfs_visit(neighbor);
            }
        }
    };

    // Начинаем обход с заданной начальной вершины
    stack.push(start_vertex);
    dfs_visit(start_vertex);

    // Выводим результат DFS в программное окно
    for (char vertex: visit_order) {
        result << vertex << " ";
    }
    this->printoutAlgorithm = result.str();

    // Анимация посещения вершин
    int delay_ms = 400; // Задержка между отрисовками вершин (в миллисекундах)
    for (char vertex: visit_order) {
        // Создаем таймер для отрисовки вершины с задержкой
        Glib::signal_timeout().connect_once([=]() {
            Canvas::getInstance()->outline_vertex(vertex, Color(0.6, 1, 0.6, 1));
            // Отключаем таймер после одного выполнения
        }, delay_ms);
        // Увеличиваем задержку перед следующей отрисовкой
        delay_ms += 400;
    }
}


void Graph::runAlgorithm(const std::string &algorithm) {
    if (algorithm == "BFS") {
        this->run_bfs('A');
    } else if (algorithm == "DFS") {
        this->run_dfs('A');
    } else {
        this->printoutAlgorithm = "Sorry, not avaible(";
    }
}