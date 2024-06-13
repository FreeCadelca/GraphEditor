#include "external/googletest-1.14.0/googletest/include/gtest/gtest.h"
#include "Graph.h"
#include "gtkmm.h"
#include "Canvas.h"

class GraphTestSuite : public ::testing::Test {
protected:
    Graph* graph;
    Gtk::Main kit;

    void SetUp() override {
        // Получаем экземпляр графа
        graph = Graph::getInstance();

        // Очистка состояния графа
        graph->adjacent_list.clear();
        graph->coords.clear();
        graph->ID_NEXT_TITLE = 0;

        // Любая дополнительная инициализация
        graph->printoutAlgorithm.clear();
    }

    void TearDown() override {
        // Очистка состояния графа после теста, если необходимо
        graph->adjacent_list.clear();
        graph->coords.clear();
        graph->ID_NEXT_TITLE = 0;
    }
// Вспомогательная функция для проверки, что буфер заполнен белым цветом (только RGB)
    bool isBufferCleared(Cairo::RefPtr<Cairo::ImageSurface> surface) {
        int width = surface->get_width();
        int height = 1080;
        unsigned char* data = surface->get_data();
        int stride = surface->get_stride();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width * 4; x += 4) { // *4, потому что RGB (можно игнорировать альфа-канал)
                if (data[y * stride + x] != 255 ||     // R
                    data[y * stride + x + 1] != 255 || // G
                    data[y * stride + x + 2] != 255) { // B
                    return false;
                }
            }
        }
        return true;
    }

    // Вспомогательная функция для сравнения содержимого двух буферов
    bool buffersAreEqual(Cairo::RefPtr<Cairo::ImageSurface> surface1, Cairo::RefPtr<Cairo::ImageSurface> surface2) {
        int width1 = surface1->get_width();
        int height1 = surface1->get_height();
        int stride1 = surface1->get_stride();
        int width2 = surface2->get_width();
        int height2 = surface2->get_height();
        int stride2 = surface2->get_stride();

        if (width1 != width2 || height1 != height2 || stride1 != stride2) {
            return false;
        }

        unsigned char* data1 = surface1->get_data();
        unsigned char* data2 = surface2->get_data();

        for (int y = 0; y < height1; ++y) {
            for (int x = 0; x < stride1; ++x) { // Проход по строке данных
                if (data1[y * stride1 + x] != data2[y * stride2 + x]) {
                    return false;
                }
            }
        }
        return true;
    }
};

TEST_F(GraphTestSuite, TestBFS_EmptyGraph) {
    GraphTestAccessor::run_bfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBFS_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(graph->getPrintoutAlgorithm(), "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestBFS_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::run_bfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBFS_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в ширину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A "), std::string::npos);
}
TEST_F(GraphTestSuite, TestBFS_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::run_bfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBFS_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в ширину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A B C "), std::string::npos);
}

TEST_F(GraphTestSuite, TestBFS_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::run_bfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBFS_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в ширину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A B C "), std::string::npos);
}

TEST_F(GraphTestSuite, TestDFS_EmptyGraph) {
    GraphTestAccessor::run_dfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDFS_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(graph->getPrintoutAlgorithm(), "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestDFS_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::run_dfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDFS_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в глубину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A "), std::string::npos);
}

TEST_F(GraphTestSuite, TestDFS_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::run_dfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDFS_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в глубину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A B C "), std::string::npos);
}

TEST_F(GraphTestSuite, TestDFS_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::run_dfs(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDFS_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Обход в глубину начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A B C "), std::string::npos);
}


TEST_F(GraphTestSuite, TestDijkstra_EmptyGraph) {
    GraphTestAccessor::dijkstra(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDijkstra_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestDijkstra_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::dijkstra(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDijkstra_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Дейкстры начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
}

TEST_F(GraphTestSuite, TestDijkstra_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::dijkstra(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDijkstra_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Дейкстры начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
    EXPECT_NE(result.find("B: 1"), std::string::npos);
    EXPECT_NE(result.find("C: 2"), std::string::npos);
}

TEST_F(GraphTestSuite, TestDijkstra_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::dijkstra(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDijkstra_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Дейкстры начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
    EXPECT_NE(result.find("B: 1"), std::string::npos);
    EXPECT_NE(result.find("C: 2"), std::string::npos);
}

TEST_F(GraphTestSuite, TestBellmanFord_EmptyGraph) {
    GraphTestAccessor::bellman_ford(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBellmanFord_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestBellmanFord_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::bellman_ford(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBellmanFord_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Беллмана-Форда начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
}

TEST_F(GraphTestSuite, TestBellmanFord_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::bellman_ford(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBellmanFord_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Беллмана-Форда начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
    EXPECT_NE(result.find("B: 1"), std::string::npos);
    EXPECT_NE(result.find("C: 2"), std::string::npos);
}

TEST_F(GraphTestSuite, TestBellmanFord_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::bellman_ford(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBellmanFord_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Алгоритм Беллмана-Форда начиная с вершины A"), std::string::npos);
    EXPECT_NE(result.find("A: 0"), std::string::npos);
    EXPECT_NE(result.find("B: 1"), std::string::npos);
    EXPECT_NE(result.find("C: 2"), std::string::npos);
}

TEST_F(GraphTestSuite, TestKruskal_EmptyGraph) {
    GraphTestAccessor::kruskal(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestKruskal_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestKruskal_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::kruskal(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestKruskal_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph has only one vertex");
}

TEST_F(GraphTestSuite, TestKruskal_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::kruskal(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestKruskal_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Минимальное остовное дерево (алгоритм Краскала)"), std::string::npos);
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"), std::string::npos); // Adjust weight sum if needed
}

TEST_F(GraphTestSuite, TestKruskal_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::kruskal(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestKruskal_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Минимальное остовное дерево (алгоритм Краскала)"), std::string::npos);
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"), std::string::npos); // Adjust weight sum if needed
}

TEST_F(GraphTestSuite, TestPrim_EmptyGraph) {
    GraphTestAccessor::prim(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestPrim_EmptyGraph: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph is empty");
}

TEST_F(GraphTestSuite, TestPrim_SingleVertex) {
    graph->addVertex(0.0, 0.0);
    GraphTestAccessor::prim(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestPrim_SingleVertex: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Error: Graph has only one vertex");
}

TEST_F(GraphTestSuite, TestPrim_LinearGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(2.0, 0.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');

    GraphTestAccessor::prim(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestPrim_LinearGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Минимальное остовное дерево (алгоритм Прима)"), std::string::npos);
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"), std::string::npos); // Adjust weight sum if needed
}

TEST_F(GraphTestSuite, TestPrim_CyclicGraph) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C
    graph->addEdge('A', 'B');
    graph->addEdge('B', 'C');
    graph->addEdge('C', 'A');

    GraphTestAccessor::prim(*graph);
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestPrim_CyclicGraph: " << result << std::endl; // Debug output
    EXPECT_NE(result.find("Минимальное остовное дерево (алгоритм Прима)"), std::string::npos);
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"), std::string::npos); // Adjust weight sum if needed
}

TEST_F(GraphTestSuite, TestFind_SingleVertex) {
    std::map<char, char> parent;
    parent['A'] = 'A';

    char root = GraphTestAccessor::find(*graph, parent, 'A');
    EXPECT_EQ(root, 'A');
}

TEST_F(GraphTestSuite, TestFind_TwoVertices_SeparateSets) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'B';

    char rootA = GraphTestAccessor::find(*graph, parent, 'A');
    char rootB = GraphTestAccessor::find(*graph, parent, 'B');
    EXPECT_EQ(rootA, 'A');
    EXPECT_EQ(rootB, 'B');
}

TEST_F(GraphTestSuite, TestFind_TwoVertices_SameSet) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'A'; // B is connected to A

    char rootA = GraphTestAccessor::find(*graph, parent, 'A');
    char rootB = GraphTestAccessor::find(*graph, parent, 'B');
    EXPECT_EQ(rootA, 'A');
    EXPECT_EQ(rootB, 'A');
}

TEST_F(GraphTestSuite, TestFind_PathCompression) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'A'; // B is connected to A
    parent['C'] = 'B'; // C is connected to B

    char rootC = GraphTestAccessor::find(*graph, parent, 'C');
    EXPECT_EQ(rootC, 'A');
    EXPECT_EQ(parent['C'], 'A'); // Check if path compression occurred
    EXPECT_EQ(parent['B'], 'A'); // Check if path compression occurred
}

TEST_F(GraphTestSuite, TestUnionSets_SingleVertex) {
    std::map<char, char> parent;
    parent['A'] = 'A';

    GraphTestAccessor::union_sets(*graph, parent, 'A', 'A');
    EXPECT_EQ(parent['A'], 'A');
}

TEST_F(GraphTestSuite, TestUnionSets_TwoSeparateVertices) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'B';

    GraphTestAccessor::union_sets(*graph, parent, 'A', 'B');
    EXPECT_EQ(parent['B'], 'A'); // B should be connected to A
    EXPECT_EQ(parent['A'], 'A'); // A should remain its own root
}

TEST_F(GraphTestSuite, TestUnionSets_TwoConnectedVertices) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'A'; // B is already connected to A

    GraphTestAccessor::union_sets(*graph, parent, 'A', 'B');
    EXPECT_EQ(parent['B'], 'A'); // B should still be connected to A
    EXPECT_EQ(parent['A'], 'A'); // A should remain its own root
}

TEST_F(GraphTestSuite, TestUnionSets_ComplexGraph) {
    std::map<char, char> parent;
    parent['A'] = 'A';
    parent['B'] = 'B';
    parent['C'] = 'C';
    parent['D'] = 'D';

    // Create initial connections: A-B and C-D
    GraphTestAccessor::union_sets(*graph, parent, 'A', 'B');
    GraphTestAccessor::union_sets(*graph, parent, 'C', 'D');
    EXPECT_EQ(parent['B'], 'A');
    EXPECT_EQ(parent['D'], 'C');

    // Union the two sets: A-B and C-D
    GraphTestAccessor::union_sets(*graph, parent, 'A', 'C');
    EXPECT_EQ(parent['C'], 'A'); // C should now be connected to A
    EXPECT_EQ(parent['D'], 'C'); // D should still be connected to C
}





TEST_F(GraphTestSuite, ClearTempBuffer) {
    Canvas* canvas = Canvas::getInstance();
    // Предварительно заполняем временный буфер другим цветом, чтобы проверить очистку
    auto temp_ctx = canvas->get_context(canvas->temp_buffer);
    temp_ctx->set_source_rgb(1, 0, 0); // Красный цвет
    temp_ctx->paint();


    // Очищаем временный буфер
    canvas->clear_temp_buffer();

    // Проверяем, что временный буфер очищен и заполнен белым цветом
    auto temp_image_surface = Cairo::RefPtr<Cairo::ImageSurface>::cast_dynamic(canvas->temp_buffer);
    ASSERT_TRUE(temp_image_surface);
    EXPECT_TRUE(isBufferCleared(temp_image_surface));
}

// Тестирование функции update_main_buffer
TEST_F(GraphTestSuite, UpdateMainBuffer) {
    Canvas* canvas = Canvas::getInstance();
    // Заполняем временный буфер
    auto temp_ctx = canvas->get_context(canvas->temp_buffer);
    temp_ctx->set_source_rgb(1, 0, 0);
    temp_ctx->paint();

    // Обновляем основной буфер из временного буфера
    canvas->update_main_buffer();

    // Проверяем, что основной буфер обновлен из временного буфера
    auto main_image_surface = Cairo::RefPtr<Cairo::ImageSurface>::cast_dynamic(canvas->buffer);
    auto temp_image_surface = Cairo::RefPtr<Cairo::ImageSurface>::cast_dynamic(canvas->temp_buffer);
    ASSERT_TRUE(main_image_surface);
    ASSERT_TRUE(temp_image_surface);
    EXPECT_TRUE(buffersAreEqual(main_image_surface, temp_image_surface));
}

// Тестирование функции change_tool
TEST_F(GraphTestSuite, ChangeTool) {
    Canvas* canvas = Canvas::getInstance();
    // Тестируем смену на инструмент VERTEX
    canvas->change_tool(Canvas::VERTEX);
    EXPECT_EQ(canvas->getState(), Canvas::DEFAULT | Canvas::VERTEX);

    // Тестируем смену на инструмент EDGE
    canvas->change_tool(Canvas::EDGE);
    EXPECT_EQ(canvas->getState(), Canvas::DEFAULT | Canvas::EDGE);

    // Тестируем смену на инструмент DRAWING
    canvas->change_tool(Canvas::DRAWING);
    EXPECT_EQ(canvas->getState(), Canvas::DEFAULT | Canvas::DRAWING);
}

// Тесты для функции calculateArrowCoordinates
TEST_F(GraphTestSuite, CalculateArrowCoordinates) {
    Canvas* canvas = Canvas::getInstance();

    // Тест 1: Проверка вычислений при произвольных координатах
    {
        double start_x = 0;
        double start_y = 0;
        double end_x = 10;
        double end_y = 10;
        double vertex_radius = 20;

        auto result = canvas->calculateArrowCoordinates(start_x, start_y, end_x, end_y, vertex_radius);
        double new_start_x, new_start_y, new_end_x, new_end_y;
        std::tie(new_start_x, new_start_y, new_end_x, new_end_y) = result;

        double length = std::sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
        double dir_x = (end_x - start_x) / length;
        double dir_y = (end_y - start_y) / length;

        double expected_start_x = start_x + dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_start_y = start_y + dir_y * vertex_radius - dir_x * vertex_radius * 0.25;
        double expected_end_x = end_x - dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_end_y = end_y - dir_y * vertex_radius - dir_x * vertex_radius * 0.25;

        EXPECT_NEAR(new_start_x, expected_start_x, 1e-5);
        EXPECT_NEAR(new_start_y, expected_start_y, 1e-5);
        EXPECT_NEAR(new_end_x, expected_end_x, 1e-5);
        EXPECT_NEAR(new_end_y, expected_end_y, 1e-5);
    }

    // Тест 2: Проверка вычислений при вертикальных координатах
    {
        double start_x = 5;
        double start_y = 8;
        double end_x = 5;
        double end_y = 20;
        double vertex_radius = 20;

        auto result = canvas->calculateArrowCoordinates(start_x, start_y, end_x, end_y, vertex_radius);
        double new_start_x, new_start_y, new_end_x, new_end_y;
        std::tie(new_start_x, new_start_y, new_end_x, new_end_y) = result;

        double length = std::sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
        double dir_x = (end_x - start_x) / length;
        double dir_y = (end_y - start_y) / length;

        double expected_start_x = start_x + dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_start_y = start_y + dir_y * vertex_radius - dir_x * vertex_radius * 0.25;
        double expected_end_x = end_x - dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_end_y = end_y - dir_y * vertex_radius - dir_x * vertex_radius * 0.25;

        EXPECT_NEAR(new_start_x, expected_start_x, 1e-5);
        EXPECT_NEAR(new_start_y, expected_start_y, 1e-5);
        EXPECT_NEAR(new_end_x, expected_end_x, 1e-5);
        EXPECT_NEAR(new_end_y, expected_end_y, 1e-5);
    }

    // Тест 3: Проверка вычислений при горизонтальных координатах
    {
        double start_x = 67;
        double start_y = 8;
        double end_x = 5;
        double end_y = 8;
        double vertex_radius = 20;

        auto result = canvas->calculateArrowCoordinates(start_x, start_y, end_x, end_y, vertex_radius);
        double new_start_x, new_start_y, new_end_x, new_end_y;
        std::tie(new_start_x, new_start_y, new_end_x, new_end_y) = result;

        double length = std::sqrt((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y));
        double dir_x = (end_x - start_x) / length;
        double dir_y = (end_y - start_y) / length;

        double expected_start_x = start_x + dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_start_y = start_y + dir_y * vertex_radius - dir_x * vertex_radius * 0.25;
        double expected_end_x = end_x - dir_x * vertex_radius + dir_y * vertex_radius * 0.25;
        double expected_end_y = end_y - dir_y * vertex_radius - dir_x * vertex_radius * 0.25;

        EXPECT_NEAR(new_start_x, expected_start_x, 1e-5);
        EXPECT_NEAR(new_start_y, expected_start_y, 1e-5);
        EXPECT_NEAR(new_end_x, expected_end_x, 1e-5);
        EXPECT_NEAR(new_end_y, expected_end_y, 1e-5);
    }
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
