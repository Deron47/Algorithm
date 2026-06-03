#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <optional>

using namespace std;
using namespace sf;

// Размер поля 10x10, так как вариантный лабиринт имеет сетку 10 на 10
const int GRID_SIZE = 10;

// Размер одной клетки в пикселях
const int CELL_SIZE = 60;

// Размер окна программы
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE;

// Типы клеток на поле
enum CellType {
    EMPTY,      // свободная клетка
    OBSTACLE,   // препятствие
    HEAVY,      // клетка с повышенной стоимостью прохождения
    START,      // стартовая клетка
    END,        // конечная клетка
    PATH,       // найденный путь
    VISITED,    // обработанная клетка, closed list
    OPEN        // клетка в open list
};

// Структура одной клетки поля
struct Cell {
    int x, y;           // координаты клетки
    CellType type;      // тип клетки
    int f, g, h;        // значения для алгоритма A*: f = g + h
    int weight;         // стоимость прохождения клетки
    Cell* parent;       // указатель на предыдущую клетку для восстановления пути

    Cell(int x, int y) : x(x), y(y), type(EMPTY), f(0), g(0), h(0), weight(1), parent(nullptr) {}
};

// Сравнение клеток по значению f.
// Нужно для priority_queue, чтобы выбирать клетку с минимальным f.
struct CompareF {
    bool operator()(Cell* const& c1, Cell* const& c2) {
        return c1->f > c2->f;
    }
};

// Эвристика — манхэттенское расстояние.
// Используется для примерной оценки расстояния от текущей клетки до цели.
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Проверка, что координаты находятся внутри поля
bool isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// Функция отрисовки поля
void drawGrid(RenderWindow& window, vector<vector<Cell>>& grid) {
    window.clear(Color::White);

    // Проходим по всем клеткам поля
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // Создаём квадрат для отображения клетки
            RectangleShape cellShape({ (float)CELL_SIZE - 1.f, (float)CELL_SIZE - 1.f });
            cellShape.setPosition({ (float)(i * CELL_SIZE), (float)(j * CELL_SIZE) });

            // Цвет клетки зависит от её типа
            switch (grid[i][j].type) {
            case EMPTY:
                cellShape.setFillColor(Color::White);
                break;
            case OBSTACLE:
                cellShape.setFillColor(Color::Black);
                break;
            case HEAVY:
                cellShape.setFillColor(Color(210, 180, 140)); // песочный цвет
                break;
            case START:
                cellShape.setFillColor(Color(255, 165, 0)); // оранжевый
                break;
            case END:
                cellShape.setFillColor(Color(64, 224, 208)); // бирюзовый
                break;
            case VISITED:
                cellShape.setFillColor(Color::Red);
                break;
            case OPEN:
                cellShape.setFillColor(Color::Green);
                break;
            case PATH:
                cellShape.setFillColor(Color(128, 0, 128)); // фиолетовый
                break;
            }

            window.draw(cellShape);
        }
    }

    window.display();
}

// Загрузка фиксированного лабиринта для варианта 8
void loadVariant8(vector<vector<Cell>>& grid, Cell*& start, Cell*& end) {
    // Сначала очищаем поле и создаём новые пустые клетки
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = Cell(i, j);
        }
    }

    // Устанавливаем стартовую точку
    start = &grid[0][3];
    start->type = START;

    // Устанавливаем конечную точку
    end = &grid[9][9];
    end->type = END;

    // Координаты препятствий для варианта 8
    vector<pair<int, int>> obstacles = {
        {0,0}, {1,0},{2,0}, {3,0}, {4,0}, {6,0},
        {0,1}, {1,1}, {5,1},
        {1,2},
        {2,3}, {4,3}, {5,3}, {7,3}, {8,3},
        {1,4}, {3,4}, {5,4}, {6,4}, {7,4},
        {6,5}, {7,5}, {8,5}, {9,5},
        {0,6}, {4,6}, {5,6},
        {0,7}, {5,7}, {6,7}, {7,7}, {9,7},
        {8,8},
        {2,9}
    };

    // Расставляем препятствия на поле
    for (auto obs : obstacles) {
        grid[obs.first][obs.second].type = OBSTACLE;
    }
}

// Основная функция алгоритма A*
void aStar(vector<vector<Cell>>& grid, Cell* start, Cell* end, RenderWindow& window) {
    // Открытый список: клетки, которые нужно рассмотреть
    priority_queue<Cell*, vector<Cell*>, CompareF> open_list;

    // Закрытый список: клетки, которые уже были обработаны
    vector<vector<bool>> closed_list(GRID_SIZE, vector<bool>(GRID_SIZE, false));

    // Вспомогательный массив: показывает, находится ли клетка в open_list
    vector<vector<bool>> in_open_list(GRID_SIZE, vector<bool>(GRID_SIZE, false));

    // Начальная клетка имеет стоимость g = 0
    start->g = 0;

    // h — эвристическая оценка расстояния от старта до цели
    start->h = heuristic(start->x, start->y, end->x, end->y);

    // f — итоговая оценка клетки
    start->f = start->g + start->h;

    // Добавляем стартовую клетку в open list
    open_list.push(start);
    in_open_list[start->x][start->y] = true;

    // Возможные направления движения: вверх, вниз, влево, вправо
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // Пока есть клетки, которые можно рассмотреть
    while (!open_list.empty()) {
        // Берём клетку с минимальным значением f
        Cell* current = open_list.top();
        open_list.pop();

        // После извлечения клетка больше не считается находящейся в open list
        in_open_list[current->x][current->y] = false;

        // Если достигли конечной клетки, восстанавливаем путь
        if (current->x == end->x && current->y == end->y) {
            Cell* temp = current->parent;

            // Идём от конца к старту по указателям parent
            while (temp != nullptr && temp != start) {
                temp->type = PATH;
                temp = temp->parent;
            }

            return;
        }

        // Переносим текущую клетку в closed list
        closed_list[current->x][current->y] = true;

        // Отмечаем обработанную клетку красным цветом
        if (current != start && current != end) {
            current->type = VISITED;
        }

        // Проверяем четырёх соседей текущей клетки
        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            // Проверяем, что сосед находится внутри поля
            if (isValid(nx, ny)) {
                Cell* neighbor = &grid[nx][ny];

                // Препятствия и уже обработанные клетки пропускаем
                if (neighbor->type == OBSTACLE || closed_list[nx][ny]) {
                    continue;
                }

                // Новая стоимость пути до соседней клетки.
                // Здесь учитывается вес клетки.
                int tentative_g = current->g + neighbor->weight;

                // Если сосед ещё не в open list или найден более короткий путь
                if (!in_open_list[nx][ny] || tentative_g < neighbor->g) {
                    neighbor->g = tentative_g;
                    neighbor->h = heuristic(nx, ny, end->x, end->y);
                    neighbor->f = neighbor->g + neighbor->h;
                    neighbor->parent = current;

                    // Если соседней клетки ещё нет в open list, добавляем её
                    if (!in_open_list[nx][ny]) {
                        open_list.push(neighbor);
                        in_open_list[nx][ny] = true;

                        // Отмечаем клетку зелёным, если это не конечная точка
                        if (neighbor != end) {
                            neighbor->type = OPEN;
                        }
                    }
                }
            }
        }

        // Перерисовываем поле после каждого шага алгоритма
        drawGrid(window, grid);
    }
}

int main() {
    // Создаём окно SFML
    RenderWindow window(VideoMode({ (unsigned int)WINDOW_SIZE, (unsigned int)WINDOW_SIZE }), "A* Algorithm - Variant 8");

    // Создаём двумерную сетку клеток
    vector<vector<Cell>> grid(GRID_SIZE, vector<Cell>(GRID_SIZE, Cell(0, 0)));

    Cell* start = nullptr;
    Cell* end = nullptr;

    // Загружаем лабиринт варианта 8
    loadVariant8(grid, start, end);

    // Основной цикл программы
    while (window.isOpen()) {
        while (const std::optional<Event> event = window.pollEvent()) {
            // Закрытие окна
            if (event->is<Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPress = event->getIf<Event::KeyPressed>()) {
                // SPACE — запуск алгоритма A*
                if (keyPress->code == Keyboard::Key::Space) {
                    aStar(grid, start, end, window);
                }

                // V — возврат к варианту 8
                if (keyPress->code == Keyboard::Key::V) {
                    loadVariant8(grid, start, end);
                }

                // R — генерация случайного поля с препятствиями
                if (keyPress->code == Keyboard::Key::R) {
                    // Очищаем поле
                    for (int i = 0; i < GRID_SIZE; i++) {
                        for (int j = 0; j < GRID_SIZE; j++) {
                            grid[i][j] = Cell(i, j);
                        }
                    }

                    // Случайно выбираем старт и конец
                    start = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];
                    end = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];

                    // Проверяем, чтобы старт и конец не совпадали
                    while (start == end) {
                        end = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];
                    }

                    start->type = START;
                    end->type = END;

                    // Добавляем случайные препятствия
                    for (int i = 0; i < 20; i++) {
                        int rx = rand() % GRID_SIZE;
                        int ry = rand() % GRID_SIZE;

                        if (grid[rx][ry].type == EMPTY) {
                            grid[rx][ry].type = OBSTACLE;
                        }
                    }
                }

                // W — генерация случайной карты с весами
                if (keyPress->code == Keyboard::Key::W) {
                    // Очищаем поле
                    for (int i = 0; i < GRID_SIZE; i++) {
                        for (int j = 0; j < GRID_SIZE; j++) {
                            grid[i][j] = Cell(i, j);
                        }
                    }

                    // Случайно выбираем стартовую и конечную клетки
                    start = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];
                    end = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];

                    // Старт и конец не должны совпадать
                    while (start == end) {
                        end = &grid[rand() % GRID_SIZE][rand() % GRID_SIZE];
                    }

                    start->type = START;
                    end->type = END;

                    // Добавляем клетки с повышенной стоимостью прохождения
                    for (int i = 0; i < 30; i++) {
                        int rx = rand() % GRID_SIZE;
                        int ry = rand() % GRID_SIZE;

                        if (grid[rx][ry].type == EMPTY) {
                            grid[rx][ry].type = HEAVY;
                            grid[rx][ry].weight = 5;
                        }
                    }
                }
            }
        }

        // Постоянно отображаем текущее состояние поля
        drawGrid(window, grid);
    }

    return 0;
}