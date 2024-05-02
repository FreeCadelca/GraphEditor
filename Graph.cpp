//
// Created by Dmitriy on 20.04.2024.
//

#include "Graph.h"

Graph::Graph() = default;

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
        for (auto j : this->adjacent_matrix[i]) {
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

    // Пока очередь не пуста, обходим граф в ширину
    while (!q.empty()) {
        char current_vertex = q.front();
        q.pop();

        // Добавляем текущую вершину в результат обхода
        result << current_vertex << " ";

        // Получаем соседей текущей вершины из таблицы смежности
        auto neighbors = this->adjacent_list[current_vertex];
        // Для каждого соседа, если он еще не посещен, добавляем его в очередь и отмечаем как посещенный
        for (char neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }

    // Выводим результат BFS в программное окно
    this->printoutAlgorithm = result.str();
}

void Graph::dfs_util(char vertex, std::set<char>& visited, std::stringstream& result) {
    // Помечаем текущую вершину как посещенную
    visited.insert(vertex);

    // Добавляем текущую вершину в результат
    result << vertex << " ";

    // Получаем соседей текущей вершины из таблицы смежности
    auto neighbors = this->adjacent_list[vertex];

    // Для каждого соседа, если он еще не посещен, рекурсивно запускаем обход в глубину
    for (char neighbor : neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            dfs_util(neighbor, visited, result);
        }
    }
}

// Вспомогательная функция для рекурсивного обхода графа в глубину
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

    // Запускаем рекурсивную функцию DFS для поиска в глубину
    dfs_util(start_vertex, visited, result);

    // Выводим результат DFS в программное окно
    this->printoutAlgorithm = result.str();
}

void Graph::dijkstra(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата алгоритма Дейкстры
    std::stringstream result;
    result << "Алгоритм Дейкстры начиная с вершины " << start_vertex << ":\n";
    result << "Минимальные расстояния до вершин:\n";

    // Инициализируем расстояния для всех вершин как бесконечность,
    // кроме стартовой вершины, расстояние до которой равно 0
    std::vector<int> distances(TITLES.size(), std::numeric_limits<int>::max());
    //Здесь 'A' используется для того, чтобы привести символ start_vertex к
    // числовому значению, чтобы его можно было использовать как индекс в массиве distances
    distances[start_vertex - 'A'] = 0;

    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Пока не все вершины посещены
    while (visited.size() < TITLES.size()) {
        // Находим вершину с минимальным расстоянием среди непосещенных
        char current_vertex = '\0';
        int min_distance = std::numeric_limits<int>::max();
        for (char v : TITLES) {
            if (visited.find(v) == visited.end() && distances[v - start_vertex] < min_distance) {
                min_distance = distances[v - start_vertex];
                current_vertex = v;
            }
        }

        // Если не удалось найти непосещенный узел, выходим из цикла
        if (current_vertex == '\0') {
            break;
        }

        // Помечаем текущую вершину как посещенную
        visited.insert(current_vertex);

        // Обновляем расстояния до всех соседей текущей вершины
        for (char neighbor : adjacent_list[current_vertex]) {
            int weight = adjacent_matrix[current_vertex - start_vertex][neighbor - start_vertex];
            if (distances[current_vertex - start_vertex] != std::numeric_limits<int>::max() &&
                distances[current_vertex - start_vertex] + weight < distances[neighbor - start_vertex]) {
                distances[neighbor - start_vertex] = distances[current_vertex - start_vertex] + weight;
            }
        }
    }

    // Формируем строку с минимальными расстояниями только для тех вершин, до которых существует путь
    for (char v : TITLES) {
        if (distances[v - start_vertex] != std::numeric_limits<int>::max()) {
            result << v << ": " << distances[v - start_vertex] << "\n";
        }
    }

    // Выводим результат алгоритма Дейкстры в программное окно
    this->printoutAlgorithm = result.str();
}


void Graph::bellman_ford(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и ребрах в графе
    if (coords.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата алгоритма Беллмана — Форда
    std::stringstream result;
    result << "Алгоритм Беллмана — Форда начиная с вершины " << start_vertex << ":\n";
    result << "Минимальные расстояния до вершин:\n";

    // Инициализируем расстояния для всех вершин как бесконечность,
    // кроме стартовой вершины, расстояние до которой равно 0
    std::vector<int> distances(TITLES.size(), std::numeric_limits<int>::max());
    distances[start_vertex - 'A'] = 0;

    // Проходимся по всем рёбрам графа |V| - 1 раз для нахождения кратчайших расстояний
    for (int i = 0; i < TITLES.size() - 1; ++i) {
        for (char v_from : TITLES) {
            for (char v_to : adjacent_list[v_from]) {
                int weight = adjacent_matrix[v_from - 'A'][v_to - 'A'];
                if (distances[v_from - 'A'] != std::numeric_limits<int>::max() &&
                    distances[v_from - 'A'] + weight < distances[v_to - 'A']) {
                    distances[v_to - 'A'] = distances[v_from - 'A'] + weight;
                }
            }
        }
    }

    // Проверяем наличие отрицательных циклов
    for (char v_from : TITLES) {
        for (char v_to : adjacent_list[v_from]) {
            int weight = adjacent_matrix[v_from - 'A'][v_to - 'A'];
            if (distances[v_from - 'A'] != std::numeric_limits<int>::max() &&
                distances[v_from - 'A'] + weight < distances[v_to - 'A']) {
                this->printoutAlgorithm = "Ошибка: Граф содержит отрицательный цикл";
                return;
            }
        }
    }

    // Формируем строку с минимальными расстояниями
    for (char v : TITLES) {
        if (distances[v - 'A'] != std::numeric_limits<int>::max()) {
            result << v << ": " << distances[v - 'A'] << "\n";
        }
    }

    // Выводим результат алгоритма Беллмана — Форда в программное окно
    this->printoutAlgorithm = result.str();
}

// Вспомогательная функция для поиска корневой вершины дерева
char Graph::find(std::map<char, char>& parent, char vertex) {
    if (parent[vertex] != vertex) {
        parent[vertex] = find(parent, parent[vertex]);
    }
    return parent[vertex];
}

// Вспомогательная функция для объединения двух деревьев
void Graph::union_sets(std::map<char, char>& parent, char u, char v) {
    u = find(parent, u);
    v = find(parent, v);
    if (u != v) {
        parent[v] = u;
    }
}

void Graph::kruskal(char start_vertex) {
    // Проверка на наличие рёбер в графе
    if (adjacent_list.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создание вектора для хранения всех рёбер графа
    std::vector<Edge> edges;

    // Заполнение вектора рёбрами
    for (const auto& adjacent_vertex_pair : adjacent_list) {
        char from = adjacent_vertex_pair.first;
        for (char to : adjacent_vertex_pair.second) {
            edges.emplace_back(from, to, adjacent_matrix[from - start_vertex][to - start_vertex]);
        }
    }

    // Сортировка рёбер по весу
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
        return e1.weight < e2.weight;
    });

    // Создание отображения для хранения родительских вершин
    std::map<char, char> parent;
    for (char vertex = start_vertex; vertex < start_vertex + TITLES.size(); ++vertex) {
        parent[vertex] = vertex;
    }

    // Создание вектора для хранения остовного дерева
    std::vector<Edge> mst;

    // Проход по всем рёбрам в отсортированном порядке и добавление их в остовное дерево,
    // если они не создают цикл
    for (const Edge& edge : edges) {
        char root_from = find(parent, edge.v_from);
        char root_to = find(parent, edge.v_to);
        if (root_from != root_to) {
            mst.push_back(edge);
            union_sets(parent, root_from, root_to);
        }
    }

    // Формирование строки с остовным деревом и его весом
    std::stringstream result;
    result << "Минимальное остовное дерево (алгоритм Краскала):\n";
    int total_weight = 0;
    for (const Edge& edge : mst) {
        result << edge.v_from << " - " << edge.v_to << ": " << edge.weight << "\n";
        total_weight += edge.weight;
    }
    result << "Вес минимального остовного дерева равен: " << total_weight << "\n";

    // Вывод результата в программное окно
    this->printoutAlgorithm = result.str();
}




void Graph::prim(char start_vertex) {
    // Проверка на наличие рёбер в графе
    if (adjacent_list.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создание пустого множества для хранения посещённых вершин
    std::set<char> visited;

    // Создание пустого множества для хранения рёбер минимального остовного дерева
    std::set<Edge> mst;

    // Добавление стартовой вершины в множество посещённых
    visited.insert(start_vertex);

    // Пока не все вершины посещены
    while (visited.size() < adjacent_list.size()) {
        // Поиск ребра с минимальным весом, исходящего из посещённой вершины
        int min_weight = std::numeric_limits<int>::max();
        Edge min_edge{' ', ' ', std::numeric_limits<int>::max()};

        // Проходим по всем посещенным вершинам и ищем смежные рёбра с минимальным весом
        for (char from : visited) {
            for (char to : adjacent_list[from]) {
                if (visited.find(to) == visited.end() && adjacent_matrix[from - start_vertex][to - start_vertex] < min_weight) {
                    min_weight = adjacent_matrix[from - start_vertex][to - start_vertex];
                    min_edge = Edge(from, to, min_weight);
                }
            }
        }

        // Добавление найденного ребра в остовное дерево
        mst.insert(min_edge);

        // Добавление вершины, соединённой найденным ребром, в множество посещённых
        visited.insert(min_edge.v_to);
    }


    // Формирование строки с остовным деревом и его весом
    std::stringstream result;
    result << "Минимальное остовное дерево (алгоритм Прима):\n";
    int total_weight = 0;
    for (const Edge& edge : mst) {
        result << edge.v_from << " - " << edge.v_to << ": " << edge.weight << "\n";
        total_weight += edge.weight;
    }
    result << "Вес минимального остовного дерева равен: " << total_weight << "\n";

    // Вывод результата в программное окно
    this->printoutAlgorithm = result.str();
}





void Graph::runAlgorithm(const std::string& algorithm) {
    if (algorithm == "BFS") {
        this->run_bfs('A');
    } else if (algorithm == "DFS") {
        this->run_dfs('A');
    } else if (algorithm == "Djkstra") {
        this->dijkstra('A');
    } else if (algorithm == "Bellman-Ford") {
        this->bellman_ford('A');
    } else if (algorithm == "Kraskal") {
        this->kruskal('A');
    } else if (algorithm == "Prim") {
        this->prim('A');
    } else {
        this->printoutAlgorithm = "Sorry, not available(";
    }
}
