import pygame
import random
import os
import math

#  НАСТРОЙКИ

WIDTH        = 900
HEIGHT       = 620
ARRAY_SIZE   = 80
BAR_AREA_H   = 480
PANEL_Y      = 490

WHITE  = (255, 255, 255)
BLACK  = (20,  20,  20)
GRAY   = (147, 145, 148)
DARK   = (50,  50,  50)

BAR_DEFAULT = (199, 46, 255)
BAR_ACTIVE  = (255, 80,  80)
BAR_SORTED  = (80,  220, 120)
BAR_PIVOT   = (255, 210, 0)

BTN_COLOR  = (70,  70,  70)
BTN_HOVER  = (100, 100, 200)
BTN_ACTIVE = (60,  180, 100)
TEXT_COLOR = (230, 230, 230)


#  МОТИВАЦИОННЫЕ ФРАЗЫ

PHRASES = [
    "Почти готово!",
    "Ещё немного...",
    "Терпение!",
    "Алгоритм не подведёт!",
    "Хоар одобряет",
    "Отсортируем мусор на дорогах",
    "Каждый элемент на месте",
    "Вот бы сортировать так дела!",
    "Думай рекурсивно!",
    "Делим и властвуем",
    "Pivot выбран мудро",
    "Ещё пара сравнений...",
    "Вперёд, алгоритм!",
    "Работаем усердно...",
    "Кодим до 3 ночи, но никому ни слова",
    "Не вайб - вайб всюду, и не думать хромосомом",
    "Очередь за хлебом дольше...",
]

def load_phrases(path="phrases.txt"):
    if not os.path.exists(path):
        return PHRASES[:]
    try:
        with open(path, "r", encoding="utf-8") as f:
            lines = [l.strip() for l in f if l.strip()]
        return lines if lines else PHRASES[:]
    except Exception:
        return PHRASES[:]


#  ЛАВОВАЯ ТЕКСТУРА
def generate_lava_surface(w, h):
    print("Генерация лавовой текстуры...", end=" ", flush=True)
    surf = pygame.Surface((w, h))

    for y in range(h):
        for x in range(w):
            t = 1.0 - (y / h)

            noise = (
                math.sin(x * 0.25 + y * 0.08) * 0.40 +
                math.sin(x * 0.07 - y * 0.18) * 0.30 +
                math.sin(x * 0.50 + y * 0.35) * 0.15 +
                math.sin(x * 0.12 + y * 0.55) * 0.15
            )
            noise = noise * 0.5 + 0.5

            t = max(0.0, min(1.0, t * 0.65 + noise * 0.35))

            if t < 0.5:
                tt = t / 0.5
                r = int(120 + 135 * tt)
                g = int(0   + 80  * tt)
                b = 0
            else:
                tt = (t - 0.5) / 0.5
                r = 255
                g = int(80  + 165 * tt)
                b = int(0   + 50  * tt)

            surf.set_at((x, y), (r, g, b))

    print("готово!")
    return surf


#  ВСПЛЫВАЮЩАЯ ФРАЗА
class FloatingPhrase:

    LIFE_S   = 3.2
    FADE_IN  = 0.4
    FADE_OUT = 1.0
    SPEED_Y  = 28

    def __init__(self, text, x, y, font):
        self.text     = text
        self.x        = float(x)
        self.y        = float(y)
        self.font     = font
        self.elapsed  = 0.0
        self.alive    = True
        self.drift_x  = random.uniform(-8, 8)
        self._start_x = float(x)

    def update(self, dt):
        self.elapsed += dt
        if self.elapsed >= self.LIFE_S:
            self.alive = False
            return
        self.y -= self.SPEED_Y * dt
        progress = self.elapsed / self.LIFE_S
        self.x = self._start_x + self.drift_x * math.sin(progress * math.pi)

    @property
    def alpha(self):
        t = self.elapsed
        if t < self.FADE_IN:
            return int(255 * (t / self.FADE_IN))
        elif t > self.LIFE_S - self.FADE_OUT:
            remaining = self.LIFE_S - t
            return int(255 * max(0, remaining / self.FADE_OUT))
        return 255

    def draw(self, screen):
        if not self.alive:
            return
        a = self.alpha
        if a <= 0:
            return
        surf = self.font.render(self.text, True, (255, 255, 255))
        surf = surf.convert_alpha()
        surf.set_alpha(a)
        screen.blit(surf, (int(self.x), int(self.y)))


#  ГЕНЕРАЦИЯ МАССИВА
def generate_array(size):
    return [random.randint(10, BAR_AREA_H - 20) for _ in range(size)]


#  ОТРИСОВКА КАДРА
def draw(screen, arr, font, small_font,
         highlight=None, pivot=None, sorted_indices=None,
         algo_name="", comparisons=0, swaps=0, phrases=None,
         lava_surf=None, lava_offset=0):

    screen.fill(BLACK)

    if highlight      is None: highlight = []
    if sorted_indices is None: sorted_indices = set()
    if phrases        is None: phrases = []

    n     = len(arr)
    bar_w = (WIDTH - 20) // n

    for i, val in enumerate(arr):
        x = 10 + i * bar_w
        h = val
        y = BAR_AREA_H - h

        if i in highlight:         color = BAR_ACTIVE
        elif i == pivot:           color = BAR_PIVOT
        elif i in sorted_indices:  color = BAR_SORTED
        else:                      color = BAR_DEFAULT

        bw = bar_w - 1

        if color == BAR_DEFAULT and lava_surf is not None:
            src_y = (y + lava_offset) % BAR_AREA_H

            if src_y + h <= BAR_AREA_H:
                screen.blit(lava_surf, (x, y), (x, src_y, bw, h))
            else:
                part1 = BAR_AREA_H - src_y
                part2 = h - part1
                screen.blit(lava_surf, (x, y),         (x, src_y, bw, part1))
                screen.blit(lava_surf, (x, y + part1), (x, 0,     bw, part2))

            pygame.draw.line(screen, (60, 0, 0), (x,      y), (x,      y + h - 1))
            pygame.draw.line(screen, (60, 0, 0), (x+bw-1, y), (x+bw-1, y + h - 1))
            pygame.draw.line(screen, (255, 220, 80), (x+1, y), (x+bw-2, y))

        else:
            pygame.draw.rect(screen, color, (x, y, bw, h))
            dark = (max(0,color[0]-60), max(0,color[1]-60), max(0,color[2]-60))
            pygame.draw.line(screen, dark, (x,      y), (x,      y+h-1))
            pygame.draw.line(screen, dark, (x+bw-1, y), (x+bw-1, y+h-1))
            if h > 3:
                light = (min(255,color[0]+80), min(255,color[1]+80), min(255,color[2]+80))
                pygame.draw.line(screen, light, (x+1, y), (x+bw-2, y))

    pygame.draw.line(screen, GRAY, (0, BAR_AREA_H), (WIDTH, BAR_AREA_H), 1)

    pygame.draw.rect(screen, (20, 20, 20), (0, 0, WIDTH, 24))
    info = f"{algo_name}   |   Сравнений: {comparisons}   |   Перестановок: {swaps}"
    screen.blit(small_font.render(info, True, GRAY), (10, 5))

    for ph in phrases:
        ph.draw(screen)


#  КНОПКИ
def draw_buttons(screen, font, selected_algo, phrase_count):
    buttons = []

    btn_defs = [
        ("Новый массив", 10,  155, "new"),
        ("Слияние",      175, 125, "merge"),
        ("Поразрядная",  310, 125, "radix"),
        ("Быстро",       445, 125, "quick"),
        ("Старт",        580, 120, "start"),
        ("Стоп",         710, 110, "stop"),
    ]

    mouse_pos = pygame.mouse.get_pos()

    for text, x, w, action in btn_defs:
        rect = pygame.Rect(x, PANEL_Y + 10, w, 36)

        if action == selected_algo:
            color = BTN_ACTIVE

        elif rect.collidepoint(mouse_pos):
            color = BTN_HOVER
        else:
            color = BTN_COLOR

        pygame.draw.rect(screen, color, rect, border_radius=6)
        pygame.draw.rect(screen, GRAY,  rect, 1, border_radius=6)

        label = font.render(text, True, TEXT_COLOR)
        screen.blit(label, (rect.centerx - label.get_width()  // 2,
                             rect.centery - label.get_height() // 2))
        buttons.append((rect, action))

    return buttons


#  ГЕНЕРАТОРЫ СОРТИРОВОК
def merge_sort_gen(arr):
    cmp  = [0]
    swps = [0]

    def merge(arr, left, mid, right):
        L = arr[left:mid+1]
        R = arr[mid+1:right+1]
        i = j = 0
        k = left
        while i < len(L) and j < len(R):
            cmp[0] += 1
            if L[i] <= R[j]:
                arr[k] = L[i]; i += 1
            else:
                arr[k] = R[j]; j += 1
            swps[0] += 1
            yield arr[:], [k], None, cmp[0], swps[0]
            k += 1
        while i < len(L):
            arr[k] = L[i]; i += 1; k += 1; swps[0] += 1
            yield arr[:], [k-1], None, cmp[0], swps[0]
        while j < len(R):
            arr[k] = R[j]; j += 1; k += 1; swps[0] += 1
            yield arr[:], [k-1], None, cmp[0], swps[0]

    def sort(arr, left, right):
        if left < right:
            mid = (left + right) // 2
            yield from sort(arr, left, mid)
            yield from sort(arr, mid+1, right)
            yield from merge(arr, left, mid, right)

    yield from sort(arr, 0, len(arr)-1)


def radix_sort_gen(arr):
    cmp  = [0]
    swps = [0]

    if not arr:
        return

    max_val = max(arr)
    digit   = 1

    while max_val // digit > 0:
        n      = len(arr)
        output = [0] * n
        count  = [0] * 10

        for i in range(n):
            d = (arr[i] // digit) % 10
            count[d] += 1
            cmp[0] += 1
            yield arr[:], [i], None, cmp[0], swps[0]

        for i in range(1, 10):
            count[i] += count[i-1]

        for i in range(n-1, -1, -1):
            d = (arr[i] // digit) % 10
            output[count[d]-1] = arr[i]
            count[d] -= 1
            swps[0] += 1

        for i in range(n):
            arr[i] = output[i]
            yield arr[:], [i], None, cmp[0], swps[0]

        digit *= 10


def quick_sort_gen(arr):
    cmp  = [0]
    swps = [0]

    stack = [(0, len(arr)-1)]

    while stack:
        low, high = stack.pop()
        if low >= high:
            continue

        pivot_val = arr[high]
        i = low - 1

        for j in range(low, high):
            cmp[0] += 1
            yield arr[:], [j, high], high, cmp[0], swps[0]
            if arr[j] <= pivot_val:
                i += 1
                arr[i], arr[j] = arr[j], arr[i]
                swps[0] += 1
                yield arr[:], [i, j], None, cmp[0], swps[0]

        arr[i+1], arr[high] = arr[high], arr[i+1]
        swps[0] += 1
        pi = i + 1
        yield arr[:], [pi], pi, cmp[0], swps[0]

        stack.append((low,  pi - 1))
        stack.append((pi+1, high))


#  ГЛАВНАЯ ФУНКЦИЯ
def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Визуализатор сортировок — Вариант 8")

    font        = pygame.font.SysFont("Segoe UI", 17)
    small_font  = pygame.font.SysFont("Segoe UI", 15)
    phrase_font = pygame.font.SysFont("Segoe UI", 16, bold=True)

    clock = pygame.time.Clock()

    lava_surf = generate_lava_surface(WIDTH, BAR_AREA_H)

    phrases_list = load_phrases("phrases.txt")
    print(f"Загружено фраз: {len(phrases_list)}")

    arr           = generate_array(ARRAY_SIZE)
    selected_algo = "merge"
    sorting       = False
    generator     = None
    delay_ms      = 30

    highlight    = []
    pivot_idx    = None
    comparisons  = 0
    swaps        = 0
    sorted_all   = False

    active_phrases  = []
    phrase_timer_ms = 0
    PHRASE_EVERY_MS = 2800

    algo_names = {
        "merge": "Сортировка слиянием",
        "radix": "Поразрядная сортировка",
        "quick": "Быстрая сортировка",
    }

    running    = True
    prev_ticks = pygame.time.get_ticks()

    while running:
        now_ticks  = pygame.time.get_ticks()
        dt_ms      = now_ticks - prev_ticks
        dt_s       = dt_ms / 1000.0
        prev_ticks = now_ticks

        lava_offset = int(now_ticks / 50) % BAR_AREA_H

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                buttons = draw_buttons(screen, font, selected_algo, len(phrases_list))
                for rect, action in buttons:
                    if rect.collidepoint(event.pos):
                        if action == "new":
                            arr         = generate_array(ARRAY_SIZE)
                            sorting     = False
                            generator   = None
                            highlight   = []
                            pivot_idx   = None
                            comparisons = 0
                            swaps       = 0
                            sorted_all  = False
                            active_phrases.clear()

                        elif action in ("merge", "radix", "quick"):
                            selected_algo = action
                            sorting       = False
                            generator     = None
                            highlight     = []
                            pivot_idx     = None
                            comparisons   = 0
                            swaps         = 0
                            sorted_all    = False
                            active_phrases.clear()

                        elif action == "start" and not sorting:
                            sorting         = True
                            sorted_all      = False
                            work_arr        = arr[:]
                            highlight       = []
                            comparisons     = 0
                            swaps           = 0
                            phrase_timer_ms = 0
                            active_phrases.clear()

                            if selected_algo == "merge":
                                generator = merge_sort_gen(work_arr)
                            elif selected_algo == "radix":
                                generator = radix_sort_gen(work_arr)
                            elif selected_algo == "quick":
                                generator = quick_sort_gen(work_arr)

                        elif action == "stop":
                            sorting   = False
                            generator = None

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    if sorting:
                        sorting = False
                    else:
                        sorting         = True
                        sorted_all      = False
                        work_arr        = arr[:]
                        highlight       = []
                        comparisons     = 0
                        swaps           = 0
                        phrase_timer_ms = 0
                        active_phrases.clear()
                        if selected_algo == "merge":
                            generator = merge_sort_gen(work_arr)
                        elif selected_algo == "radix":
                            generator = radix_sort_gen(work_arr)
                        elif selected_algo == "quick":
                            generator = quick_sort_gen(work_arr)

                elif event.key == pygame.K_r:
                    arr         = generate_array(ARRAY_SIZE)
                    sorting     = False
                    generator   = None
                    highlight   = []
                    comparisons = 0
                    swaps       = 0
                    sorted_all  = False
                    active_phrases.clear()

                elif event.key == pygame.K_UP:
                    delay_ms = max(delay_ms - 10, 5)
                elif event.key == pygame.K_DOWN:
                    delay_ms = min(delay_ms + 10, 500)
                elif event.key == pygame.K_l:
                    phrases_list = load_phrases("phrases.txt")
                    print(f"Перезагружено фраз: {len(phrases_list)}")

        if sorting and generator is not None:
            try:
                state = next(generator)
                arr, highlight, pivot_idx, comparisons, swaps = state
            except StopIteration:
                sorting    = False
                generator  = None
                highlight  = []
                pivot_idx  = None
                sorted_all = True

        if sorting:
            phrase_timer_ms += dt_ms
            if phrase_timer_ms >= PHRASE_EVERY_MS:
                phrase_timer_ms = 0
                px   = random.randint(30, WIDTH - 150)
                py   = random.randint(BAR_AREA_H // 4, BAR_AREA_H - 60)
                text = random.choice(phrases_list)
                active_phrases.append(FloatingPhrase(text, px, py, phrase_font))

        for ph in active_phrases:
            ph.update(dt_s)
        active_phrases = [ph for ph in active_phrases if ph.alive]

        algo_label = algo_names.get(selected_algo, "")
        if sorted_all:
            algo_label += "  ✓ ГОТОВО"

        draw(screen, arr, font, small_font,
             highlight=highlight,
             pivot=pivot_idx,
             sorted_indices=set(range(len(arr))) if sorted_all else set(),
             algo_name=algo_label,
             comparisons=comparisons,
             swaps=swaps,
             phrases=active_phrases,
             lava_surf=lava_surf,
             lava_offset=lava_offset)

        draw_buttons(screen, font, selected_algo, len(phrases_list))

        hint = small_font.render(
            "Space — старт/стоп   R — новый массив   ↑/↓ — скорость   L — перезагрузить фразы",
            True, (80, 80, 80)
        )
        screen.blit(hint, (10, HEIGHT - 22))

        pygame.display.flip()
        clock.tick(1000 // max(delay_ms, 1))

    pygame.quit()


if __name__ == "__main__":
    main()