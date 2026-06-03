import pygame
import random
from queue import PriorityQueue

pygame.init()

WIDTH = 600
GRID_SIZE = 10
CELL_SIZE = WIDTH // GRID_SIZE
WIN = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("Алгоритм A* - Вариант 8")

# Цвета
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)  # Препятствие
ORANGE = (255, 165, 0)  # Старт
TURQUOISE = (64, 224, 208)  # Конец
GREEN = (0, 255, 0)  # В очереди (Open list)
RED = (255, 0, 0)  # Посещена (Closed list)
PURPLE = (128, 0, 128)  # Итоговый путь
GREY = (128, 128, 128)  # Линии сетки
BROWN = (210, 180, 140)  # Клетка с весом (труднопроходимая)


class Cell:
    def __init__(self, x, y):
        self.x_idx = x
        self.y_idx = y
        self.x_coord = x * CELL_SIZE
        self.y_coord = y * CELL_SIZE
        self.color = WHITE
        self.weight = 1
        self.neighbors = []

    def get_pos(self):
        return self.x_idx, self.y_idx

    def is_obstacle(self):
        return self.color == BLACK

    def reset(self):
        self.color = WHITE
        self.weight = 1

    def make_start(self):
        self.color = ORANGE
        self.weight = 1

    def make_end(self):
        self.color = TURQUOISE
        self.weight = 1

    def make_obstacle(self):
        self.color = BLACK
        self.weight = 1

    def make_heavy(self):
        self.color = BROWN
        self.weight = 5

    def make_open(self):
        self.color = GREEN

    def make_closed(self):
        self.color = RED

    def make_path(self):
        self.color = PURPLE

    def draw(self, win):
        pygame.draw.rect(win, self.color, (self.x_coord, self.y_coord, CELL_SIZE, CELL_SIZE))

    def update_neighbors(self, grid):
        self.neighbors = []
        # Вниз (y + 1)
        if self.y_idx < GRID_SIZE - 1 and not grid[self.x_idx][self.y_idx + 1].is_obstacle():
            self.neighbors.append(grid[self.x_idx][self.y_idx + 1])
        # Вверх (y - 1)
        if self.y_idx > 0 and not grid[self.x_idx][self.y_idx - 1].is_obstacle():
            self.neighbors.append(grid[self.x_idx][self.y_idx - 1])
        # Вправо (x + 1)
        if self.x_idx < GRID_SIZE - 1 and not grid[self.x_idx + 1][self.y_idx].is_obstacle():
            self.neighbors.append(grid[self.x_idx + 1][self.y_idx])
        # Влево (x - 1)
        if self.x_idx > 0 and not grid[self.x_idx - 1][self.y_idx].is_obstacle():
            self.neighbors.append(grid[self.x_idx - 1][self.y_idx])


def h(p1, p2):
    x1, y1 = p1
    x2, y2 = p2
    return abs(x1 - x2) + abs(y1 - y2)


def reconstruct_path(came_from, current, draw):
    while current in came_from:
        current = came_from[current]
        current.make_path()
        draw()


def a_star_algorithm(draw, grid, start, end):
    count = 0
    open_set = PriorityQueue()
    open_set.put((0, count, start))
    came_from = {}

    g_score = {cell: float("inf") for row in grid for cell in row}
    g_score[start] = 0

    f_score = {cell: float("inf") for row in grid for cell in row}
    f_score[start] = h(start.get_pos(), end.get_pos())

    open_set_hash = {start}

    while not open_set.empty():

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()

        current = open_set.get()[2]
        open_set_hash.remove(current)

        if current == end:
            reconstruct_path(came_from, end, draw)
            end.make_end()
            start.make_start()
            return True

        for neighbor in current.neighbors:
            temp_g_score = g_score[current] + neighbor.weight

            if temp_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = temp_g_score
                f_score[neighbor] = temp_g_score + h(neighbor.get_pos(), end.get_pos())

                if neighbor not in open_set_hash:
                    count += 1
                    open_set.put((f_score[neighbor], count, neighbor))
                    open_set_hash.add(neighbor)
                    neighbor.make_open()

        draw()
        if current != start:
            current.make_closed()

    return False


def make_grid():
    grid = []
    for i in range(GRID_SIZE):
        grid.append([])
        for j in range(GRID_SIZE):
            cell = Cell(i, j)  # i = x, j = y
            grid[i].append(cell)
    return grid


def draw_grid_lines(win):
    for i in range(GRID_SIZE):
        pygame.draw.line(win, GREY, (0, i * CELL_SIZE), (WIDTH, i * CELL_SIZE))
        pygame.draw.line(win, GREY, (i * CELL_SIZE, 0), (i * CELL_SIZE, WIDTH))


def draw(win, grid):
    for row in grid:
        for cell in row:
            cell.draw(win)
    draw_grid_lines(win)
    pygame.display.update()


def load_variant_8(grid):
    # Точные координаты Варианта 8
    obstacles = [
        (0, 0), (1, 0), (2, 0), (3, 0), (4, 0), (6, 0),
        (0, 1), (1, 1), (5, 1),
        (1, 2), (6, 2), (7, 2),
        (2, 3), (4, 3), (5, 3), (7, 3), (8, 3),
        (1, 4), (3, 4), (5, 4), (6, 4), (7, 4),
        (6, 5), (7, 5), (8, 5), (9, 5),
        (0, 6), (4, 6), (5, 6),
        (0, 7), (5, 7), (6, 7), (7, 7), (9, 7),
        (8, 8),
        (2, 9)
    ]
    for x, y in obstacles:
        grid[x][y].make_obstacle()


def main():
    grid = make_grid()

    # Старт X=0, Y=3 | Финиш X=9, Y=9
    start = grid[0][3]
    end = grid[9][9]

    load_variant_8(grid)
    start.make_start()
    end.make_end()

    run = True
    while run:
        draw(WIN, grid)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    for row in grid:
                        for cell in row:
                            cell.update_neighbors(grid)
                    a_star_algorithm(lambda: draw(WIN, grid), grid, start, end)

                if event.key == pygame.K_c:
                    for row in grid:
                        for cell in row:
                            if cell.color in [GREEN, RED, PURPLE]:
                                cell.reset()
                    start.make_start()
                    end.make_end()

                if event.key == pygame.K_1:
                    start = grid[0][3]
                    end = grid[9][9]
                    for row in grid:
                        for cell in row:
                            cell.reset()
                    load_variant_8(grid)
                    start.make_start()
                    end.make_end()

                if event.key == pygame.K_2:
                    for row in grid:
                        for cell in row:
                            cell.reset()
                    start = grid[random.randint(0, 9)][random.randint(0, 9)]
                    end = grid[random.randint(0, 9)][random.randint(0, 9)]
                    while start == end:
                        end = grid[random.randint(0, 9)][random.randint(0, 9)]
                    start.make_start()
                    end.make_end()
                    for _ in range(20):
                        x, y = random.randint(0, 9), random.randint(0, 9)
                        cell = grid[x][y]
                        if cell != start and cell != end:
                            cell.make_obstacle()

                if event.key == pygame.K_3:
                    for row in grid:
                        for cell in row:
                            cell.reset()
                    start = grid[random.randint(0, 9)][random.randint(0, 9)]
                    end = grid[random.randint(0, 9)][random.randint(0, 9)]
                    while start == end:
                        end = grid[random.randint(0, 9)][random.randint(0, 9)]
                    start.make_start()
                    end.make_end()
                    for _ in range(30):
                        x, y = random.randint(0, 9), random.randint(0, 9)
                        cell = grid[x][y]
                        if cell != start and cell != end:
                            cell.make_heavy()

    pygame.quit()


if __name__ == "__main__":
    main()