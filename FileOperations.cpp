#include "FileOperations.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "json.hpp"

using json = nlohmann::json;

std::vector<std::vector<int>> cargarsudoku(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    json j;
    file >> j;

    std::vector<std::vector<int>> sudoku = j["board"].get<std::vector<std::vector<int>>>();


    const int n = sudoku.size();
    const int boxSize = static_cast<int>(sqrt(n));
    const int cellWidth = 2;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            std::cout << std::setw(cellWidth) << sudoku[row][col] << " ";
            if ((col + 1) % boxSize == 0 && col + 1 != n) {
                std::cout << "| ";
            }
        }
        std::cout << std::endl;

        if ((row + 1) % boxSize == 0 && row + 1 != n) {
            std::cout << std::string(cellWidth * n + (n / boxSize - 1) * 2, '-') << "\n";
        }
    }

    return sudoku;
}

void guardarsudoku(const std::string& filename, const std::vector<std::vector<int>>& grid, int n, double elapsedTime) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("No se pudo crear el archivo: " + filename);
    }

    json j;
    j["sudoku"] = grid;
    j["El tamaño del sudoku es:"] = std::to_string(n) + "x" + std::to_string(n);
    j["El tiempo de ejecucion:"] = elapsedTime;

    file << j.dump(4);
    std::cout << "Solucion guardada en el archivo: " << filename << "\n";
}
