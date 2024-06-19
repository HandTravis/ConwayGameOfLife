#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to read grid from file
std::vector<std::vector<bool>> readGridFromFile(const std::string& filename, int gridSize) {
    std::vector<std::vector<bool>> grid(gridSize, std::vector<bool>(gridSize, false));
    std::ifstream infile(filename);

    if (infile.is_open()) {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                int value;
                if (infile >> value) {
                    grid[i][j] = (value == 1);
                } else {
                    // Handle error while reading from file
                    std::cerr << "Error reading from file: " << filename << std::endl;
                    return grid;
                }
            }
        }
        infile.close();
    } else {
        // Handle error opening file
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return grid;
}

// Function to compare two grids
void compareGrids(const std::vector<std::vector<bool>>& grid1, const std::vector<std::vector<bool>>& grid2, int gridSize) {
    bool different = false;

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            if (grid1[i][j] != grid2[i][j]) {
                std::cout << "Difference at coordinate (" << i << ", " << j << "): ";
                std::cout << "Grid 1: " << (grid1[i][j] ? "1" : "0") << ", Grid 2: " << (grid2[i][j] ? "1" : "0") << std::endl;
                different = true;
            }
        }
    }

    if (!different) {
        std::cout << "Grids are identical." << std::endl;
    }
}

int main() {
    // Get grid size from user
    int gridSize;
    std::cout << "Enter the grid size: ";
    std::cin >> gridSize;

    // Check if the grid size is valid
    if (gridSize <= 0) {
        std::cerr << "Invalid grid size. Please enter a positive integer." << std::endl;
        return 1;
    }

    // Get filenames from user
    std::string filename1, filename2;
    std::cout << "Enter the name of the first file: ";
    std::cin >> filename1;
    std::cout << "Enter the name of the second file: ";
    std::cin >> filename2;

    // Read grids from files
    std::vector<std::vector<bool>> grid1 = readGridFromFile(filename1, gridSize);
    std::vector<std::vector<bool>> grid2 = readGridFromFile(filename2, gridSize);

    // Compare grids
    if (grid1.size() != gridSize || grid2.size() != gridSize || grid1[0].size() != gridSize || grid2[0].size() != gridSize) {
        std::cerr << "Grid size mismatch." << std::endl;
        return 1;
    }

    compareGrids(grid1, grid2, gridSize);

    return 0;
}

