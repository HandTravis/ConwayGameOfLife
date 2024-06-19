#include <vector>
#include <thread>

using namespace::std;

void thread_compute(std::vector<std::vector<bool>>& grid, std::vector<std::vector<bool>>& newGrid, int gridSize, int num_threads, int thread_id) {
    int start_row = (gridSize * thread_id) / num_threads;
    int end_row = (gridSize * (thread_id + 1)) / num_threads;

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int aliveNeighbors = 0;

            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0) continue;

                    int neighborX = i + x;
                    int neighborY = j + y;

                    if (neighborX >= 0 && neighborX < gridSize && neighborY >= 0 && neighborY < gridSize) {
                        if (grid[neighborX][neighborY]) {
                            aliveNeighbors++;
                        }
                    }
                }
            }

            if (grid[i][j]) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    newGrid[i][j] = false;
                } else {
                    newGrid[i][j] = true;
                }
            } else {
                if (aliveNeighbors == 3) {
                    newGrid[i][j] = true;
                }
            }
        }
    }
}

void updateGrid(std::vector<std::vector<bool>>& grid, int gridSize, int numThreads) {
    std::vector<std::vector<bool>> newGrid(gridSize, std::vector<bool>(gridSize, false));
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(thread_compute, std::ref(grid), std::ref(newGrid), gridSize, numThreads, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    grid = newGrid;
}
