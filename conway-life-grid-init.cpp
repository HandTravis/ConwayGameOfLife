#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <sstream> //  std::istringstream
#include <thread>


#define MAX_THREADS 16

void generateGrid(std::vector<std::vector<bool>>& grid, int gridSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < gridSize; ++i) {
        std::vector<bool> row;
        for (int j = 0; j < gridSize; ++j) {
            row.push_back(dis(gen));
        }
        grid.push_back(row);
    }
}

void saveGridToFile(const std::vector<std::vector<bool>>& grid, int gridSize, const std::string& filename) {
    std::ofstream outfile(filename);

    if (outfile.is_open()) {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                outfile << (grid[i][j] ? "1 " : "0 ");
            }
            outfile << "\n";
        }

        outfile.close();
        std::cout << "Initial state saved to file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void readInitialStateFromFile(std::vector<std::vector<bool>>& grid, int gridSize, const std::string& filename) {
    std::ifstream infile(filename);
    
    if (infile.is_open()) {
        grid.clear();
        std::string line;
        while (std::getline(infile, line)) {
            std::vector<bool> row;
            std::istringstream iss(line);
            int value;
            while (iss >> value) {
                row.push_back(value == 1);
            }
            grid.push_back(row);
        }
        infile.close();
        std::cout << "Initial state read from file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void printGrid(const std::vector<std::vector<bool>>& grid) {
    for (const auto& row : grid) {
        for (bool cell : row) {
            std::cout << (cell ? " ■ " : "   ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

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


//non threaded
void updateGrid_non_threaded(std::vector<std::vector<bool>>& grid, int GRID_SIZE_X) {
    std::vector<std::vector<bool>> newGrid(GRID_SIZE_X, std::vector<bool>(GRID_SIZE_X, false));

    for (int i = 0; i < GRID_SIZE_X; ++i) {
        for (int j = 0; j < GRID_SIZE_X; ++j) {
            int aliveNeighbors = 0;

            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0) continue;

                    int neighborX = i + x;
                    int neighborY = j + y;

                    if (neighborX >= 0 && neighborX < GRID_SIZE_X && neighborY >= 0 && neighborY < GRID_SIZE_X) {
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

    grid = newGrid;
}



int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <gridSize> <numThreads>" << std::endl;
        return 1;
    }

    int gridSize = std::stoi(argv[1]);
    int numThreads = std::stoi(argv[2]);

    if (gridSize % 4 != 0 || (numThreads != 1 && numThreads != 4 && numThreads != 16)) {
        std::cerr << "Grid size must be a multiple of 4 and numThreads must be 1, 4, or 16." << std::endl;
        return 1;
    }

    std::vector<std::vector<bool>> grid;
    generateGrid(grid, gridSize);
    saveGridToFile(grid, gridSize, "conway_initial_state_" + std::to_string(gridSize) + ".txt");

    // Read initial state from file
    //readInitialStateFromFile(grid, gridSize, "conway_initial_state_" + std::to_string(gridSize) + ".txt");

    for (int i = 0; i < 5; i++) {
        updateGrid(grid, gridSize, numThreads);
    }
    saveGridToFile(grid, gridSize, "conway_threaded.txt");

    readInitialStateFromFile(grid, gridSize, "conway_initial_state_" + std::to_string(gridSize) + ".txt");
    for (int i = 0; i < 5; i++) {
        updateGrid_non_threaded(grid, gridSize);
    }
    saveGridToFile(grid, gridSize, "conway_life.txt");

    // Call updateGrid function with the initial state
    //updateGrid(grid, gridSize, numThreads);

    return 0;
}

