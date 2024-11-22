#include <iostream>
#include <string>
#include <chrono>
#include "FileOperations.h"
#include "sudokuSolver.h"
#include "Utils.h"

int main() {
    try {
        std::string inputFilePath, outputFilePath;

        // Solicitar la ruta completa del archivo de entrada
        std::cout << "\nIngrese la ruta completa del archivo de entrada (incluyendo el nombre y extensión .json): ";
        std::getline(std::cin, inputFilePath);

        // Solicitar la ruta completa del archivo de salida
        std::cout << "Ingrese la ruta completa del archivo de salida (incluyendo el nombre y extensión .json): ";
        std::getline(std::cin, outputFilePath);

        std::cout << "\nArchivo cargado desde: " << inputFilePath << "\n";

        // Cargar el Sudoku desde el archivo
        std::vector<std::vector<int>> grid = cargarsudoku(inputFilePath);

        int n = grid.size();

        auto start = std::chrono::high_resolution_clock::now();

        // Resolver el Sudoku
        if (sudokuparalelizado(grid, n)) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            // Mostrar el Sudoku resuelto
            std::cout << "\nLa solucion encontrada es:\n";
            printSolvedSudoku(grid);

            // Guardar el Sudoku resuelto en el archivo de salida
            guardarsudoku(outputFilePath, grid, n, elapsed.count());
            std::cout << "\n El tiempo de ejecucion es: " << elapsed.count() << " segundos\n";
        } else {
            std::cout << "No se encontro solucion para el sudoku.\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}