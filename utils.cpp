#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include "json.hpp"

void printSolvedSudoku(const std::vector<std::vector<int>>& grid) {
    const int n = grid.size();
    const int boxSize = static_cast<int>(sqrt(n));
    const int cellWidth = 2;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (grid[row][col] == 0) {
                std::cout << std::setw(cellWidth) << "0 ";
            } else {
                std::cout << std::setw(cellWidth) << grid[row][col] << " ";
            }

            if ((col + 1) % boxSize == 0 && col + 1 != n) {
                std::cout << "| ";
            }
        }
        std::cout << std::endl;

        if ((row + 1) % boxSize == 0 && row + 1 != n) {
            std::cout << std::string(cellWidth * n + (n / boxSize - 1) * 2, '-') << "\n";
        }
    }
}
