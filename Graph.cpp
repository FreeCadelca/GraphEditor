//
// Created by Dmitriy on 20.04.2024.
//

#include "Graph.h"

Graph *Graph::instance = nullptr;

Graph::Graph() = default;

Graph *Graph::getInstance() {
    if (instance == nullptr) {
        instance = new Graph();
    }
    return instance;
}

void Graph::addVertex(double x, double y) {
    this->coords[TITLES[ID_NEXT_TITLE]] = {x, y}; // Добавляем новую вершину туда, куда нужно
    this->adjacent_list[TITLES[ID_NEXT_TITLE]] = {};

    if (ID_NEXT_TITLE == 0) {
        this->adjacent_matrix = std::vector<std::vector<int>>(1, std::vector<int>(1, 0));
    } else {
        adjacent_matrix.resize(adjacent_matrix.size() + 1, std::vector<int>(adjacent_matrix.size(), 0));
        for (auto &i : adjacent_matrix) {
            i.resize(i.size() + 1, 0);
        }
    }
    ID_NEXT_TITLE++;

    // Перерисовываем все элементы
    Canvas::getInstance()->redraw_all();
}

void Graph::addEdge(char v_from, char v_to) {
    this->adjacent_list[v_from].push_back(v_to);
    int index_v_from = (int)v_from - 65;
    int index_v_to = (int)v_to - 65;
    this->adjacent_matrix[index_v_from][index_v_to] = this->nextWeight;

    // Перерисовываем все элементы
    Canvas::getInstance()->redraw_all();
}


std::string Graph::getPrintoutAdjList() {
    std::string output; // Инициализируем строку для вывода
    int count_vertexes = (int) this->adjacent_list.size(); // Получаем количество вершин в графе

    // Проходимся по списку смежности и формируем строку для каждой вершины
    for (auto i : this->adjacent_list) {
        char vertex = i.first; // Получаем текущую вершину
        output.push_back(vertex); // Добавляем вершину в строку
        output += ":"; // Добавляем разделитель после вершины

        // Добавляем смежные вершины к текущей вершине
        for (auto edge : i.second) {
            output += " "; // Добавляем пробел перед каждой смежной вершиной
            output.push_back(edge); // Добавляем смежную вершину в строку
        }

        // Если список смежных вершин пуст, добавляем "None" в строку
        if (i.second.empty()) {
            output += " None"; // Добавляем информацию о пустом списке смежности
        }

        output += ";\n"; // Добавляем символ новой строки в конце строки смежности
    }

    // Возвращаем сформированную строку списка смежности
    return output;
}

std::string Graph::getPrintoutAdjMatrix() {
    // Получаем количество вершин в графе
    int count_vertexes = (int) this->adjacent_list.size();
    // Инициализируем строку для вывода
    std::string output = "    ";

    // Формируем заголовок матрицы смежности
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i]; // Добавляем заголовки вершин
        output += ' '; // Добавляем пробел между заголовками
    }
    output += '\n'; // Переходим на следующую строку после заголовка

    // Заполняем строки матрицы смежности
    for (int i = 0; i < count_vertexes; i++) {
        output += this->TITLES[i]; // Добавляем заголовок текущей вершины
        output += ": "; // Добавляем разделитель
        // Добавляем значения в строку, используя матрицу смежности
        for (auto j : this->adjacent_matrix[i]) {
            output += std::to_string(j) + " "; // Преобразуем числа в строки и добавляем их в вывод
        }
        output += '\n'; // Переходим на следующую строку после строки смежности текущей вершины
    }

    // Возвращаем сформированную строку матрицы смежности
    return output;
}

std::string Graph::getPrintoutAlgorithm() {
    return this->printoutAlgorithm;
}

void Graph::run_bfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и рёбрах в графе
    if (this->coords.empty()) {
        // Если контейнер coords пуст, устанавливаем сообщение об ошибке и завершаем функцию.
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата обхода в ширину (BFS)
    std::stringstream result;
    // Записываем информацию о начале выполнения алгоритма BFS с указанной стартовой вершиной в строковый поток.
    result << "Обход в ширину начиная с вершины " << start_vertex << ":\n";
    result << "Посещенные вершины: ";

    // Очередь для обхода графа в ширину
    std::queue<char> q;
    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Начинаем обход с заданной начальной вершины
    q.push(start_vertex);
    visited.insert(start_vertex);

    // Отрисовка первой вершины сразу
    Canvas::getInstance()->outline_vertex(start_vertex, Color(0.6, 1, 0.6, 1));
    // Добавляем небольшую задержку перед началом анимации
    usleep(400000);

    // Задержка между отрисовками вершин (в миллисекундах)
    int delay_ms = 400;

    // Обработка вершин с задержкой
    while (!q.empty()) {
        // Получаем текущую вершину из очереди
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

                // Создаем таймер для отложенной отрисовки вершины с задержкой
                Glib::signal_timeout().connect([=]() {
                    // Вызываем метод outline_vertex класса Canvas для отображения посещенной вершины
                    Canvas::getInstance()->outline_vertex(neighbor, Color(0.6, 1, 0.6, 1));
                    return false; // Отключаем таймер после одного выполнения
                }, delay_ms);

                // Увеличиваем задержку перед следующей отрисовкой
                delay_ms += 400;
            }
        }
    }

    // Присваиваем строковое представление потока result переменной printoutAlgorithm для вывода результата обхода в ширину.
    this->printoutAlgorithm = result.str();
}

void Graph::run_dfs(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и рёбрах в графе
    if (coords.empty()) {
        // Если контейнер coords пуст, устанавливаем сообщение об ошибке и завершаем функцию.
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата обхода в глубину (DFS)
    std::stringstream result;
    // Записываем информацию о начале выполнения алгоритма DFS с указанной стартовой вершиной в строковый поток.
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
        // Помечаем вершину как посещенную и добавляем её в список посещенных вершин
        visited.insert(vertex);
        visit_order.push_back(vertex);
        // Получаем список соседей текущей вершины
        auto neighbors = this->adjacent_list[vertex];
        // Перебираем соседей
        for (char neighbor: neighbors) {
            // Если соседняя вершина не была посещена ранее, добавляем её в стек для дальнейшего обхода
            if (visited.find(neighbor) == visited.end()) {
                stack.push(neighbor);
                // Рекурсивно вызываем функцию обхода для соседней вершины
                dfs_visit(neighbor);
            }
        }
    };

    // Начинаем обход с заданной начальной вершины
    stack.push(start_vertex);
    dfs_visit(start_vertex);

    // Формируем строку с порядком посещения вершин
    for (char vertex: visit_order) {
        result << vertex << " ";
    }

    // Присваиваем строковое представление потока result переменной printoutAlgorithm для вывода результата обхода в глубину.
    this->printoutAlgorithm = result.str();

    // Анимация посещения вершин
    int delay_ms = 400; // Задержка между отрисовками вершин (в миллисекундах)
    for (char vertex: visit_order) {
        // Создаем таймер для отрисовки вершины с задержкой
        Glib::signal_timeout().connect_once([=]() {
            // Вызываем метод outline_vertex класса Canvas для отображения посещенной вершины
            Canvas::getInstance()->outline_vertex(vertex, Color(0.6, 1, 0.6, 1));
            // Отключаем таймер после одного выполнения
        }, delay_ms);
        // Увеличиваем задержку перед следующей отрисовкой
        delay_ms += 400;
    }
}

void Graph::dijkstra(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и рёбрах в графе
    if (coords.empty()) {
        // Если контейнер coords пуст, устанавливаем сообщение об ошибке и завершаем функцию.
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата алгоритма Дейкстры
    std::stringstream result;
    // Записываем информацию о начале выполнения алгоритма с указанной стартовой вершиной в строковый поток.
    result << "Алгоритм Дейкстры начиная с вершины " << start_vertex << ":\n";
    result << "Минимальные расстояния до вершин:\n";

    // Инициализируем расстояния для всех вершин как бесконечность,
    // кроме стартовой вершины, расстояние до которой равно 0
    std::vector<int> distances(TITLES.size(), std::numeric_limits<int>::max());
    // Здесь 'A' используется для того, чтобы привести символ start_vertex к
    // числовому значению, чтобы его можно было использовать как индекс в массиве distances
    distances[start_vertex - 'A'] = 0;

    // Множество для отслеживания посещенных вершин
    std::set<char> visited;

    // Задержка между отрисовками вершин (в миллисекундах)
    int delay_ms = 400;

    // Пока не все вершины посещены
    while (visited.size() < TITLES.size()) {
        // Находим вершину с минимальным расстоянием среди непосещенных
        char current_vertex = '\0';
        int min_distance = std::numeric_limits<int>::max();
        for (char v : TITLES) {
            // Перебираем все вершины графа
            if (visited.find(v) == visited.end() && distances[v - start_vertex] < min_distance) {
                // Если вершина v не посещена и расстояние от стартовой вершины до v меньше текущего минимального расстояния,
                // обновляем значения current_vertex и min_distance
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
                // Если расстояние до соседней вершины меньше текущего значения, обновляем расстояние.
                distances[neighbor - start_vertex] = distances[current_vertex - start_vertex] + weight;
                // Создаем таймер для отложенной анимации обновления расстояния
                Glib::signal_timeout().connect([=]() {
                    // Подсвечиваем обновленное ребро
                    Canvas::getInstance()->redraw_edge(current_vertex, neighbor, Color(1, 0.6, 0.6, 1));
                    // Подсвечиваем вершину
                    Canvas::getInstance()->outline_vertex(neighbor, Color(0.6, 1, 0.6, 1));
                    Glib::signal_timeout().connect([=]() {
                        Canvas::getInstance()->outline_vertex(neighbor, Color(0.5, 0.5, 0.5, 1));
                        return false;
                    }, delay_ms);
                    return false; // Отключаем таймер после одного выполнения
                }, delay_ms);

                // Увеличиваем задержку перед следующей анимацией
                delay_ms += 400;
            }
        }
    }

    // Формируем строку с минимальными расстояниями только для тех вершин, до которых существует путь
    for (char v : TITLES) {
        if (distances[v - start_vertex] != std::numeric_limits<int>::max()) {
            // Записываем информацию о минимальном расстоянии от начальной вершины до текущей вершины v.
            result << v << ": " << distances[v - start_vertex] << "\n";
        }
    }

    // Присваиваем строковое представление потока result переменной printoutAlgorithm для вывода результата алгоритма.
    this->printoutAlgorithm = result.str();
}


void Graph::bellman_ford(char start_vertex) {
    // Проверяем, есть ли данные о вершинах и рёбрах в графе
    if (coords.empty()) {
        // Если контейнер coords пуст, устанавливаем сообщение об ошибке и завершаем функцию.
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создаем строковый поток для формирования результата алгоритма Беллмана — Форда
    std::stringstream result;
    // Записываем информацию о начале выполнения алгоритма с указанной стартовой вершины в строковый поток.
    result << "Алгоритм Беллмана — Форда начиная с вершины " << start_vertex << ":\n";
    result << "Минимальные расстояния до вершин:\n";

    // Инициализируем расстояния для всех вершин как бесконечность, кроме стартовой вершины, расстояние до которой равно 0
    std::vector<int> distances(TITLES.size(), std::numeric_limits<int>::max());
    distances[start_vertex - 'A'] = 0;

    // Проходимся по всем рёбрам графа |V| - 1 раз для нахождения кратчайших расстояний
    // Проходим по всем вершинам графа для обновления расстояний на каждой итерации
    for (int i = 0; i < TITLES.size() - 1; ++i) {
        // Внутренний цикл: перебираем все вершины графа
        for (char v_from : TITLES) {
            // Для каждой вершины перебираем все смежные с ней вершины
            for (char v_to : adjacent_list[v_from]) {
                // Получаем вес ребра между вершинами v_from и v_to
                int weight = adjacent_matrix[v_from - 'A'][v_to - 'A'];

                // Проверяем, что расстояние до вершины v_from не равно бесконечности
                // и обновляем расстояние до вершины v_to, если новое расстояние меньше текущего
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
                // Если обнаружен отрицательный цикл, устанавливаем сообщение об ошибке и завершаем функцию.
                this->printoutAlgorithm = "Ошибка: Граф содержит отрицательный цикл";
                return;
            }
        }
    }

    // Формируем строку с минимальными расстояниями
    for (char v : TITLES) {
        if (distances[v - 'A'] != std::numeric_limits<int>::max()) {
            // Записываем информацию о минимальном расстоянии от начальной вершины до текущей вершины v.
            result << v << ": " << distances[v - 'A'] << "\n";
        }
    }

    // Присваиваем строковое представление потока result переменной printoutAlgorithm для вывода результата алгоритма.
    this->printoutAlgorithm = result.str();
}

// Вспомогательная функция для поиска корневой вершины дерева
char Graph::find(std::map<char, char>& parent, char vertex) {
    // Если текущая вершина не является корнем, выполняем поиск рекурсивно
    if (parent[vertex] != vertex) {
        // Применяем сжатие пути для оптимизации: делаем родителем вершины корень её поддерева
        parent[vertex] = find(parent, parent[vertex]);
    }
    // Возвращаем корневую вершину
    return parent[vertex];
}

// Вспомогательная функция для объединения двух деревьев
void Graph::union_sets(std::map<char, char>& parent, char u, char v) {
    // Находим корневую вершину для вершины u
    u = find(parent, u);
    // Находим корневую вершину для вершины v
    v = find(parent, v);
    // Если корни разные, объединяем деревья, делая корень v поддеревом корня u
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
            // Добавляем ребро в вектор с весом, полученным из матрицы смежности
            edges.emplace_back(from, to, adjacent_matrix[from - 'A'][to - 'A']);
        }
    }

    // Сортировка рёбер по весу
    std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
        return e1.weight < e2.weight;
    });

    // Создание отображения для хранения родительских вершин (для алгоритма Union-Find)
    std::map<char, char> parent;
    for (char vertex = 'A'; vertex < 'A' + TITLES.size(); ++vertex) {
        parent[vertex] = vertex;
    }

    // Создание вектора для хранения рёбер минимального остовного дерева
    std::vector<Edge> mst;

    // Проход по всем рёбрам в отсортированном порядке и добавление их в остовное дерево,
    // если они не создают цикл
    for (const Edge& edge : edges) {
        char root_from = find(parent, edge.v_from); // Поиск корня для вершины v_from
        char root_to = find(parent, edge.v_to);     // Поиск корня для вершины v_to
        if (root_from != root_to) { // Если вершины не в одном множестве, добавить ребро в остовное дерево
            mst.push_back(edge);
            union_sets(parent, root_from, root_to); // Объединение множеств
        }
    }

    // Формирование строки с остовным деревом и его весом
    std::stringstream result;
    result << "Минимальное остовное дерево (алгоритм Краскала):\n";
    int total_weight = 0;
///АКНАЗАР
    // Визуальное выделение рёбер и вершин остовного дерева
    for (const Edge& edge : mst) {
        // Добавление информации о ребре в результат
        result << edge.v_from << " - " << edge.v_to << ": " << edge.weight << "\n";
        total_weight += edge.weight;

        int delay_ms = 400; // Задержка между отрисовками вершин (в миллисекундах)

        Glib::signal_timeout().connect_once([=]() {
            // Визуальное выделение рёбер и вершин остовного дерева
            Canvas::getInstance()->outline_vertex(edge.v_from, Color(1, 0, 0, 1)); // Красный цвет
            Canvas::getInstance()->outline_vertex(edge.v_to, Color(1, 0, 0, 1));   // Красный цвет
            Canvas::getInstance()->redraw_edge(edge.v_from, edge.v_to, Color(0.75, 0, 0, 1)); // Красный цвет

        }, delay_ms);
        delay_ms += 400;

    }
///
    result << "Вес минимального остовного дерева равен: " << total_weight << "\n";

    // Вывод результата в программное окно
    this->printoutAlgorithm = result.str();

    // Перерисовываем холст, чтобы отобразить изменения
    //Canvas::getInstance()->queue_draw();
}


void Graph::prim(char start_vertex) {
    // Проверка на наличие рёбер в графе
    if (adjacent_list.empty()) {
        this->printoutAlgorithm = "Error: Graph is empty";
        return;
    }

    // Создание пустого множества для хранения посещённых вершин
    std::set<char> visited;

    // Создание минимальной кучи для хранения рёбер (сортировка по весу)
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> edges;

    // Добавление рёбер, исходящих из стартовой вершины, в кучу
    for (char neighbor : adjacent_list[start_vertex]) {
        int weight = adjacent_matrix[start_vertex - 'A'][neighbor - 'A']; // Получение веса ребра из матрицы смежности
        edges.push(Edge(start_vertex, neighbor, weight)); // Добавление ребра в кучу
    }

    // Добавление стартовой вершины в множество посещённых вершин
    visited.insert(start_vertex);

    // Создание вектора для хранения рёбер минимального остовного дерева
    std::vector<Edge> mst;

    // Пока не все вершины посещены и в куче есть рёбра
    while (!edges.empty() && visited.size() < adjacent_list.size()) {
        // Извлечение ребра с минимальным весом из кучи
        Edge min_edge = edges.top();
        edges.pop();

        // Если конечная вершина ребра уже посещена, пропустить это ребро
        if (visited.find(min_edge.v_to) != visited.end()) {
            continue;
        }

        // Добавление найденного ребра в остовное дерево
        mst.push_back(min_edge);

        // Добавление вершины, соединённой найденным ребром, в множество посещённых вершин
        visited.insert(min_edge.v_to);

        // Добавление всех рёбер, исходящих из новой посещённой вершины, в кучу
        for (char neighbor : adjacent_list[min_edge.v_to]) {
            if (visited.find(neighbor) == visited.end()) { // Только если вершина ещё не посещена
                int weight = adjacent_matrix[min_edge.v_to - 'A'][neighbor - 'A']; // Получение веса ребра из матрицы смежности
                edges.push(Edge(min_edge.v_to, neighbor, weight)); // Добавление ребра в кучу
            }
        }
    }

    // Формирование строки с остовным деревом и его весом
    std::stringstream result;
    result << "Минимальное остовное дерево (алгоритм Прима):\n";
    int total_weight = 0;
    for (const Edge& edge : mst) {
        result << edge.v_from << " - " << edge.v_to << ": " << edge.weight << "\n"; // Добавление информации о ребре в результат
        total_weight += edge.weight; // Подсчёт общего веса остовного дерева
    }
    result << "Вес минимального остовного дерева равен: " << total_weight << "\n"; // Добавление общего веса в результат
///АКНАЗАР
// Визуальное выделение рёбер и вершин остовного дерева
    for (const Edge& edge : mst) {
        result << edge.v_from << " - " << edge.v_to << ": " << edge.weight << "\n";
        total_weight += edge.weight;

        int delay_ms = 400; // Задержка между отрисовками вершин (в миллисекундах)

        Glib::signal_timeout().connect_once([=]() {
            // Визуальное выделение рёбер и вершин остовного дерева
            Canvas::getInstance()->outline_vertex(edge.v_from, Color(1, 0, 0, 1)); // Красный цвет
            Canvas::getInstance()->outline_vertex(edge.v_to, Color(1, 0, 0, 1));   // Красный цвет
            Canvas::getInstance()->redraw_edge(edge.v_from, edge.v_to, Color(0.75, 0, 0, 1)); // Красный цвет
        }, delay_ms);
        delay_ms += 400;
    }
    ///

    // Вывод результата в программное окно
    this->printoutAlgorithm = result.str(); // Сохранение результата в поле класса
}


void Graph::runAlgorithm(const std::string& algorithm, char start_vertex) {
    printf("ran algorithm with st vertex: %c\n", start_vertex);
    if (algorithm == "BFS") {
        this->run_bfs(start_vertex);
    } else if (algorithm == "DFS") {
        this->run_dfs(start_vertex);
    } else if (algorithm == "Djkstra") {
        this->dijkstra(start_vertex);
    } else if (algorithm == "Bellman-Ford") {
        this->bellman_ford(start_vertex);
    } else if (algorithm == "Kraskal") {
        this->kruskal('A');
    } else if (algorithm == "Prim") {
        this->prim('A');
    } else {
        this->printoutAlgorithm = "Sorry, not available(";
    }
}
