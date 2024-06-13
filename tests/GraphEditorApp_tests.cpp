//
// Created by Dmitriy on 06.06.2024.
//

#include <sstream>
#include <iostream>
#include "gtest/gtest.h"
#include "gtkmm.h"
#include "../src/GraphEditorApp/GraphEditorApp.h"


class AppTestSuite : public ::testing::Test {
protected:
    std::stringstream buffer;
    std::streambuf *previousCoutBuffer{};

    GraphEditorApp *graphEditorApp{};

    void SetUp() override {
        int argc = 0;
        char **argv = nullptr;
        gtk_init(&argc, &argv);
        auto app = Gtk::Application::create(argc, argv);

        graphEditorApp = new GraphEditorApp();

        // Очистка состояния графа
        Graph::getInstance()->adjacent_list.clear();
        Graph::getInstance()->coords.clear();
        Graph::getInstance()->ID_NEXT_TITLE = 0;

        // Любая дополнительная инициализация
        Graph::getInstance()->printoutAlgorithm.clear();

        previousCoutBuffer = std::cout.rdbuf(buffer.rdbuf()); // Перенаправление std::cout
    }

    void TearDown() override {
        Graph::getInstance()->adjacent_list.clear();
        Graph::getInstance()->coords.clear();
        Graph::getInstance()->ID_NEXT_TITLE = 0;

        std::cout.rdbuf(previousCoutBuffer); // Восстановление std::cout
    }

    static void SetDefaultGraph() {
        Graph::getInstance()->addVertex(0.0, 0.0); // A
        Graph::getInstance()->addVertex(1.0, 1.0); // B
        Graph::getInstance()->addVertex(2.0, 0.0); // C
        Graph::getInstance()->addEdge('A', 'B');
        Graph::getInstance()->addEdge('B', 'A');
        Graph::getInstance()->addEdge('B', 'C');
        Graph::getInstance()->addEdge('C', 'B');
    }
};

TEST_F(AppTestSuite, HandlingNewWeightSuccess) {
    EXPECT_NE(AppTestSuite::graphEditorApp->handling_new_weight("123ab"), "123ab");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight("123ab"), "");
    EXPECT_NE(AppTestSuite::graphEditorApp->handling_new_weight("-123ab"), "-123ab");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight("-123ab"), "");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight("-123"), "-123");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight("0"), "0");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight("1234"), "1234");
    EXPECT_EQ(AppTestSuite::graphEditorApp->handling_new_weight(""), "");
}

TEST_F(AppTestSuite, PrintAlgorithmSuccess) {
    graphEditorApp->setAlgorithmName("BFS");

    graphEditorApp->print_algorithm("B");
    EXPECT_EQ(buffer.str(), "Entered wrong vertices!\n");
    buffer.str(""); // Очистка буфера для следующей проверки

    SetDefaultGraph();

    graphEditorApp->print_algorithm("B");
    EXPECT_EQ(buffer.str(), "Ran algorithm BFS with start vertex B;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();
    // Еще раз вызываем функцию, потому что
    // после успешного запуска повторный вызов этой функции будет закрывать алгоритм


    graphEditorApp->setAlgorithmName("DFS");
    graphEditorApp->print_algorithm("D");
    EXPECT_EQ(buffer.str(), "Entered wrong vertices!\n");
    buffer.str("");

    graphEditorApp->print_algorithm("C");
    EXPECT_EQ(buffer.str(), "Ran algorithm DFS with start vertex C;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();


    graphEditorApp->setAlgorithmName("Bellman-Ford");
    graphEditorApp->print_algorithm("DF");
    EXPECT_EQ(buffer.str(), "Entered wrong vertices!\n");
    buffer.str("");

    graphEditorApp->print_algorithm("CA");
    EXPECT_EQ(buffer.str(), "Ran algorithm Bellman-Ford, C->A;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();


    graphEditorApp->setAlgorithmName("Dijkstra");
    graphEditorApp->print_algorithm("DF");
    EXPECT_EQ(buffer.str(), "Entered wrong vertices!\n");
    buffer.str("");

    graphEditorApp->print_algorithm("CA");
    EXPECT_EQ(buffer.str(), "Ran algorithm Dijkstra, C->A;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();


    graphEditorApp->setAlgorithmName("Prim");
    graphEditorApp->print_algorithm();
    EXPECT_EQ(buffer.str(), "Ran algorithm Prim;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();


    graphEditorApp->setAlgorithmName("Kruskal");
    graphEditorApp->print_algorithm();
    EXPECT_EQ(buffer.str(), "Ran algorithm Kruskal;\n");
    buffer.str("");
    graphEditorApp->print_algorithm();
}

TEST_F(AppTestSuite, SetAlgorithmNameSuccess) {
    std::vector<std::string> algorithm_names = {"BFS", "DFS", "Bellman-Ford", "Dijkstra", "Prim", "Kruskal"};
    for (const auto &i: algorithm_names) {
        graphEditorApp->setAlgorithmName(i);
        EXPECT_EQ(graphEditorApp->getAlgorithmName(), i);
    }
    graphEditorApp->setAlgorithmName("Non-exist algorithm");
    EXPECT_NE(graphEditorApp->getAlgorithmName(), "Non-exist algorithm");
    EXPECT_EQ(graphEditorApp->getAlgorithmName(), "Kruskal");
}

TEST_F(AppTestSuite, GetCenterOfWindowSuccess) {
    std::pair<int, int> cur_pos;

    // Значения окна по умолчанию:
    // Положение: (0, 0)
    // Размер: (1280, 720)

    cur_pos = graphEditorApp->getCenterOfWindow();
    EXPECT_EQ(cur_pos.first, 640);
    EXPECT_EQ(cur_pos.second, 360);

    graphEditorApp->move(1000, 1000);
    cur_pos = graphEditorApp->getCenterOfWindow();
    EXPECT_EQ(cur_pos.first, 1640);
    EXPECT_EQ(cur_pos.second, 1360);

    graphEditorApp->move(500, 500);
    graphEditorApp->resize(1500, 1500);
    cur_pos = graphEditorApp->getCenterOfWindow();
    EXPECT_EQ(cur_pos.first, 1250);
    EXPECT_EQ(cur_pos.second, 1250);

    graphEditorApp->move(98, 254);
    graphEditorApp->resize(1337, 878);
    cur_pos = graphEditorApp->getCenterOfWindow();
    EXPECT_EQ(cur_pos.first, 766);
    EXPECT_EQ(cur_pos.second, 693);
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
