#include "gtest/gtest.h"
#include "../src/Graph/Graph.h"

class GraphTestSuite : public ::testing::Test {
protected:
    Graph *graph;

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

TEST_F(GraphTestSuite, TestDijkstra_NegativeCycle) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C

    // Добавляем рёбра с весами, создающими отрицательный цикл
    graph->addEdge('A', 'B', 1);
    graph->addEdge('B', 'C', -1);
    graph->addEdge('C', 'A', -1);

    GraphTestAccessor::dijkstra(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestDijkstra_NegativeCycle: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Ошибка: Граф содержит отрицательный цикл");
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

TEST_F(GraphTestSuite, TestBellmanFord_NegativeCycle) {
    graph->addVertex(0.0, 0.0); // A
    graph->addVertex(1.0, 0.0); // B
    graph->addVertex(0.0, 1.0); // C

    // Добавляем рёбра с весами, создающими отрицательный цикл
    graph->addEdge('A', 'B', 1);
    graph->addEdge('B', 'C', -1);
    graph->addEdge('C', 'A', -1);

    GraphTestAccessor::bellman_ford(*graph, 'A');
    std::string result = graph->getPrintoutAlgorithm();
    std::cout << "Result for TestBellmanFord_NegativeCycle: " << result << std::endl; // Debug output
    EXPECT_EQ(result, "Ошибка: Граф содержит отрицательный цикл");
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
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"),
              std::string::npos); // Adjust weight sum if needed
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
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"),
              std::string::npos); // Adjust weight sum if needed
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
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"),
              std::string::npos); // Adjust weight sum if needed
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
    EXPECT_NE(result.find("Вес минимального остовного дерева равен: 2"),
              std::string::npos); // Adjust weight sum if needed
}

TEST_F(GraphTestSuite, TestFind_SingleVertex) {
    std::map<char, char> parent;
    parent['A'] = 'A';

    char root = GraphTestAccessor::find(*graph, parent, 'A');
    EXPECT_EQ(root, 'A');
}

TEST_F(GraphTestSuite, TestFind_InvalidVertex) {
    std::map<char, char> parent;
    parent['A'] = 'A';

    EXPECT_THROW(GraphTestAccessor::find(*graph, parent, 'B'), std::invalid_argument);
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

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
