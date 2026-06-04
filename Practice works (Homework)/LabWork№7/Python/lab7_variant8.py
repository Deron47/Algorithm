# Задача 4. Разные деревья поиска
# Количество разных бинарных деревьев поиска с числами от 1 до n
# считается по формуле чисел Каталана.

def count_search_trees(n):
    dp = [0] * (n + 1)

    dp[0] = 1

    if n >= 1:
        dp[1] = 1

    for nodes_count in range(2, n + 1):
        total = 0

        for left_count in range(nodes_count):
            right_count = nodes_count - 1 - left_count
            total += dp[left_count] * dp[right_count]

        dp[nodes_count] = total

    return dp[n]



# Задача 7. Поле с цветочками
# Черепаха идёт из левого нижнего угла в правый верхний.
# За один ход можно идти только вверх или вправо.

def max_flowers(field):
    n = len(field)
    m = len(field[0])

    dp = [[0] * m for _ in range(n)]

    for i in range(n - 1, -1, -1):
        for j in range(m):
            best_previous = 0

            if i + 1 < n:
                best_previous = max(best_previous, dp[i + 1][j])

            if j - 1 >= 0:
                best_previous = max(best_previous, dp[i][j - 1])

            if i == n - 1 and j == 0:
                dp[i][j] = field[i][j]
            else:
                dp[i][j] = best_previous + field[i][j]

    return dp[0][m - 1]


# Задача 2. Первое и последнее вхождение числа
# Используется бинарный поиск, потому что массив отсортирован.

def find_first(nums, target):
    left = 0
    right = len(nums) - 1
    answer = -1

    while left <= right:
        mid = (left + right) // 2

        if nums[mid] == target:
            answer = mid
            right = mid - 1
        elif nums[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return answer


def find_last(nums, target):
    left = 0
    right = len(nums) - 1
    answer = -1

    while left <= right:
        mid = (left + right) // 2

        if nums[mid] == target:
            answer = mid
            left = mid + 1
        elif nums[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return answer


# Задача 1. Расстановка ферзей
# Используется поиск с возвратом.
# Ферзи ставятся по строкам.

def solve_n_queens(n):
    board = [["." for _ in range(n)] for _ in range(n)]
    result = []

    columns = [False] * n
    diag1 = [False] * (2 * n)
    diag2 = [False] * (2 * n)

    def backtrack(row):
        if row == n:
            solution = ["".join(board[i]) for i in range(n)]
            result.append(solution)
            return

        for col in range(n):
            d1 = row + col
            d2 = row - col + n

            if columns[col] or diag1[d1] or diag2[d2]:
                continue

            board[row][col] = "Q"
            columns[col] = True
            diag1[d1] = True
            diag2[d2] = True

            backtrack(row + 1)

            board[row][col] = "."
            columns[col] = False
            diag1[d1] = False
            diag2[d2] = False

    backtrack(0)
    return result



def run_tests():
    print("Запуск тестов...")

    assert count_search_trees(2) == 2
    assert count_search_trees(3) == 5
    assert count_search_trees(4) == 14

    field1 = [
        [1, 0, 1],
        [1, 1, 0]
    ]
    assert max_flowers(field1) == 3

    field2 = [
        [1, 0, 0],
        [1, 1, 0],
        [0, 0, 1]
    ]
    assert max_flowers(field2) == 2

    nums = [2, 5, 5, 5, 6, 6, 8, 9, 9, 9]
    assert find_first(nums, 5) == 1
    assert find_last(nums, 5) == 3
    assert find_first(nums, 4) == -1
    assert find_last(nums, 4) == -1

    queens_4 = solve_n_queens(4)
    assert len(queens_4) == 2

    print("Все тесты пройдены успешно.")



def task_trees():
    n = int(input("Введите n: "))
    print(count_search_trees(n))


def task_flowers():
    n, m = map(int, input("Введите n и m: ").split())
    field = []

    print("Введите поле из 0 и 1:")
    for _ in range(n):
        row = input().strip()
        field.append([int(ch) for ch in row])

    print(max_flowers(field))


def task_binary_search():
    n = int(input("Введите размер массива: "))
    nums = list(map(int, input("Введите отсортированный массив: ").split()))
    target = int(input("Введите искомое число: "))

    first = find_first(nums, target)
    last = find_last(nums, target)

    if first == -1:
        print("Element not found in the array")
    else:
        print("The first occurrence of element", target, "is located at index", first)
        print("The last occurrence of element", target, "is located at index", last)


def task_queens():
    n = int(input("Введите N: "))
    solutions = solve_n_queens(n)

    if not solutions:
        print("Решений нет")
        return

    print("Количество решений:", len(solutions))
    print("Первая найденная расстановка:")

    for row in solutions[0]:
        print(row)


def main():
    while True:
        print()
        print("Лабораторная работа №7, вариант 8")
        print("1 - Разные деревья поиска")
        print("2 - Поле с цветочками")
        print("3 - Первое и последнее вхождение числа")
        print("4 - Расстановка ферзей")
        print("5 - Запустить тесты")
        print("0 - Выход")

        choice = input("Выберите задачу: ")

        if choice == "1":
            task_trees()
        elif choice == "2":
            task_flowers()
        elif choice == "3":
            task_binary_search()
        elif choice == "4":
            task_queens()
        elif choice == "5":
            run_tests()
        elif choice == "0":
            break
        else:
            print("Неверный выбор")


if __name__ == "__main__":
    main()
