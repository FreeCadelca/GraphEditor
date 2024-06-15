/**
 * @file Graph.h
 * @brief Загаловочный файл для Graph.cpp. Реализация методов класса Graph.
 *
 * * @authors
 * София Бердус: Алгоритмы, матрица смежности, таблица смежности, список ребёр.
 */

#ifndef GRAPHEDITOR_GRAPH_H
#define GRAPHEDITOR_GRAPH_H

#include <unistd.h>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <cstdlib>
#include <sstream>
#include "../Color/Color.h"
#include "../../src/Canvas/Canvas.h"

/**
 * @brief Класс для тестирования приватных методов класса Graph.
 */
class GraphTestSuite;
class AppTestSuite;


/**
 * @brief Класс, представляющий граф и реализующий основные алгоритмы работы с графами.
 */
class Graph {
private:
    static Graph* instance; ///< Указатель на единственный экземпляр класса (singleton)
    Graph(); ///< Приватный конструктор для реализации singleton

    /**
     * @brief Структура для представления ребра.
     */
    struct Edge {
        char v_from; ///< Вершина, из которой исходит ребро
        char v_to; ///< Вершина, в которую входит ребро
        int weight; ///< Вес ребра

        /**
         * @brief Конструктор структуры Edge.
         * @param from Вершина, из которой исходит ребро
         * @param to Вершина, в которую входит ребро
         * @param w Вес ребра
         */
        Edge(char from, char to, int w) : v_from(from), v_to(to), weight(w) {}

        bool operator>(const Edge& other) const {
            return weight > other.weight;
        }

        bool operator<(const Edge& other) const {
            return weight < other.weight;
        }

        bool operator==(const Edge& other) const {
            return v_from == other.v_from && v_to == other.v_to && weight == other.weight;
        }

        bool operator!=(const Edge& other) const {
            return !(*this == other);
        }
    };


    std::string printoutAlgorithm; ///< Строка для вывода результатов алгоритмов

    void run_bfs(char start_vertex);

    void run_dfs(char start_vertex);

    void dijkstra(char start_vertex, char end_vertex = '\n');

    void bellman_ford(char start_vertex, char end_vertex = '\n');

    void kruskal();

    char find(std::map<char, char> &parent, char vertex);

    void union_sets(std::map<char, char> &parent, char u, char v);

    void prim();

    friend class GraphTestSuite;
    friend class GraphTestAccessor;

    friend class AppTestSuite;

public:
    std::map<char, std::vector<char>> adjacent_list; ///< Список смежности
//    std::map<char, std::map<char, int>> adjacent_matrix; // Матрица смежности
    std::vector<std::vector<int>> adjacent_matrix; ///< Матрица смежности
//    std::vector<WeightedEdge> adjacent; // Список рёбер
    const std::string TITLES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";///< Названия вершин
    int ID_NEXT_TITLE = 0; ///< Номер следующей вершины для выбора
    std::map<char, std::pair<int, int>> coords; ///< Координаты вершин
    int nextWeight = 1; ///< Вес следующего ребра

    /**
     * @brief Получение единственного экземпляра класса Graph.
     * @return Указатель на единственный экземпляр класса Graph.
     */
    static Graph *getInstance();

    /**
     * @brief Добавление вершины в граф.
     * @param x Координата x вершины
     * @param y Координата y вершины
     */
    void addVertex(double x, double y);

    /**
     * @brief Добавление ребра в граф.
     * @param v_from Вершина, из которой исходит ребро
     * @param v_to Вершина, в которую входит ребро
     * @param weight Вес ребра (по умолчанию -1000000000)
     */
    void addEdge(char v_from, char v_to, int weight = -1000000000);

    /**
     * @brief Получение строки, представляющей список смежности графа.
     * @return Строка, представляющая список смежности графа.
     */
    std::string getPrintoutAdjList();

    /**
     * @brief Получение строки, представляющей матрицу смежности графа.
     * @return Строка, представляющая матрицу смежности графа.
     */
    std::string getPrintoutAdjMatrix();

    /**
     * @brief Получение строки, представляющей результат выполнения алгоритма.
     * @return Строка, представляющая результат выполнения алгоритма.
     */
    std::string getPrintoutAlgorithm();

    /**
     * @brief Выполнение указанного алгоритма.
     * @param algorithm Название алгоритма
     * @param start_vertex Начальная вершина (по умолчанию '\n')
     * @param end_vertex Конечная вершина (по умолчанию '\n')
     */
    void runAlgorithm(const std::string& algorithm, char start_vertex = '\n', char end_vertex = '\n');
};

/**
 * @brief Класс для тестирования приватных методов класса Graph.
 */
class GraphTestAccessor {
public:
    static void run_bfs(Graph& graph, char start_vertex) {
        graph.run_bfs(start_vertex);
    }
    static void run_dfs(Graph& graph, char start_vertex) {
        graph.run_dfs(start_vertex);
    }
    static void dijkstra(Graph& graph, char start_vertex) {
        graph.dijkstra(start_vertex);
    }
    static void bellman_ford(Graph& graph, char start_vertex) {
        graph.bellman_ford(start_vertex);
    }
    static void kruskal(Graph& graph) {
        graph.kruskal();
    }
    static void prim(Graph& graph) {
        graph.prim();
    }
    static char find(Graph& graph, std::map<char, char> &parent, char vertex) {
        return graph.find(parent, vertex);
    }
    static void union_sets(Graph& graph, std::map<char, char> &parent, char u, char v) {
        graph.union_sets(parent, u, v);
    }
};


#endif //GRAPHEDITOR_GRAPH_H
