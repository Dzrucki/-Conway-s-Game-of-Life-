#include <vector>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

bool running = true;
bool paused = true;

std::vector<std::vector<bool>> grid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
std::vector<std::vector<bool>> newGrid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x]) {
                SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int countNeighbors(int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int ny = y + i;
            int nx = x + j;
            if (ny >= 0 && ny < GRID_HEIGHT && nx >= 0 && nx < GRID_WIDTH && grid[ny][nx]) {
                count++;
            }
        }
    }
    return count;
}

void updateCells() {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int neighbors = countNeighbors(y, x);
            if (grid[y][x]) {
                newGrid[y][x] = neighbors == 2 || neighbors == 3;
            } else {
                newGrid[y][x] = neighbors == 3;
            }
        }
    }
    grid = newGrid;
}

void handleEvents(SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                paused = !paused;
            } else if (event.key.keysym.sym == SDLK_r) {
                grid = std::vector<std::vector<bool>>(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
                paused = true;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            int gridX = x / CELL_SIZE;
            int gridY = y / CELL_SIZE;
            if (event.button.button == SDL_BUTTON_LEFT) {
                grid[gridY][gridX] = !grid[gridY][gridX];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Event event;
    while (running) {
        handleEvents(event);
        if (!paused) {
            updateCells();
        }
        drawGrid(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
