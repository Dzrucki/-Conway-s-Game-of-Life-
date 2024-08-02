import pygame
import numpy as np

# Инициализация pygame
pygame.init()

# Определение цветов
black = (0, 0, 0)
white = (255, 255, 255)

# Размеры экрана
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('Game of Life')

# Размеры ячеек
cell_size = 10

# Количество ячеек
cols, rows = width // cell_size, height // cell_size

# Создание массива для ячеек
grid = np.zeros((rows, cols), dtype=int)

# Функция для рисования сетки
def draw_grid():
    for y in range(0, height, cell_size):
        for x in range(0, width, cell_size):
            rect = pygame.Rect(x, y, cell_size, cell_size)
            pygame.draw.rect(screen, black, rect, 1)

# Функция для рисования ячеек
def draw_cells():
    for y in range(rows):
        for x in range(cols):
            if grid[y, x] == 1:
                rect = pygame.Rect(x * cell_size, y * cell_size, cell_size, cell_size)
                pygame.draw.rect(screen, white, rect)

# Функция для обновления состояния ячеек
def update_cells():
    global grid
    new_grid = grid.copy()
    for y in range(rows):
        for x in range(cols):
            # Соседние ячейки
            neighbors = [
                grid[(y - 1) % rows, (x - 1) % cols],
                grid[(y - 1) % rows, x],
                grid[(y - 1) % rows, (x + 1) % cols],
                grid[y, (x - 1) % cols],
                grid[y, (x + 1) % cols],
                grid[(y + 1) % rows, (x - 1) % cols],
                grid[(y + 1) % rows, x],
                grid[(y + 1) % rows, (x + 1) % cols],
            ]
            total = sum(neighbors)
            if grid[y, x] == 1:
                if total < 2 or total > 3:
                    new_grid[y, x] = 0
            else:
                if total == 3:
                    new_grid[y, x] = 1
    grid = new_grid

# Основной цикл программы
running = True
pause = False
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                pause = not pause
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = pygame.mouse.get_pos()
            grid[y // cell_size, x // cell_size] = 1 - grid[y // cell_size, x // cell_size]

    if not pause:
        update_cells()

    screen.fill(black)
    draw_cells()
    draw_grid()
    pygame.display.flip()
    pygame.time.delay(100)

pygame.quit()
