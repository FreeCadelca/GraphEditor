#include "external/googletest-1.14.0/googletest/include/gtest/gtest.h"
#include "Canvas.h"

class CanvasTestSuite : public ::testing::Test {
protected:
    Canvas *canvas{};

    void SetUp() override {
        canvas = Canvas::getInstance();
    }

    void TearDown() override {

    }

    // Вспомогательная функция для проверки, что буфер заполнен белым цветом (только RGB)
    static bool isBufferCleared(const Cairo::RefPtr<Cairo::ImageSurface> &surface) {
        int width = surface->get_width();
        int height = 1080;
        unsigned char *data = surface->get_data();
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
    static bool buffersAreEqual(const Cairo::RefPtr<Cairo::ImageSurface> &surface1,
                                const Cairo::RefPtr<Cairo::ImageSurface> &surface2) {
        int width1 = surface1->get_width();
        int height1 = surface1->get_height();
        int stride1 = surface1->get_stride();
        int width2 = surface2->get_width();
        int height2 = surface2->get_height();
        int stride2 = surface2->get_stride();

        if (width1 != width2 || height1 != height2 || stride1 != stride2) {
            return false;
        }

        unsigned char *data1 = surface1->get_data();
        unsigned char *data2 = surface2->get_data();

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


TEST_F(CanvasTestSuite, ClearTempBuffer) {
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
TEST_F(CanvasTestSuite, UpdateMainBuffer) {
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
TEST_F(CanvasTestSuite, ChangeTool) {
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

// Проверка вычислений при произвольных координатах
TEST_F(CanvasTestSuite, CalculateArrowCoordinates) {
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

// Проверка вычислений при вертикальных координатах
TEST_F(CanvasTestSuite, CalculateArrowCoordinates_Vertical) {
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

// Проверка вычислений при горизонтальных координатах
TEST_F(CanvasTestSuite, CalculateArrowCoordinates_Horizontal) {
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

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

