#include "sudokuSolver.h"
#include <cmath>
#include <omp.h>
#include <vector>
#include <iostream>

bool sudokuparalelizado(std::vector<std::vector<int>>& grid, int n) {
    int numeroprocesadores = omp_get_num_procs();
    omp_set_num_threads(numeroprocesadores);

    int tamañomatriz = sqrt(n);
    bool solucion = false;

    // Función de backtracking
    auto backtracking = [&](std::vector<std::vector<int>>& localGrid,
                            std::vector<std::vector<bool>>& localRows,
                            std::vector<std::vector<bool>>& localCols,
                            std::vector<std::vector<bool>>& localBoxes,
                            auto& self, int fila, int col) -> bool {
        if (fila == n) return true;
        if (col == n) return self(localGrid, localRows, localCols, localBoxes, self, fila + 1, 0);
        if (localGrid[fila][col] != 0) return self(localGrid, localRows, localCols, localBoxes, self, fila, col + 1);

        int boxIndex = (fila / tamañomatriz) * tamañomatriz + (col / tamañomatriz);
        for (int num = 1; num <= n; num++) {
            if (!localRows[fila][num] && !localCols[col][num] && !localBoxes[boxIndex][num]) {
                localRows[fila][num] = localCols[col][num] = localBoxes[boxIndex][num] = true;
                localGrid[fila][col] = num;

                if (self(localGrid, localRows, localCols, localBoxes, self, fila, col + 1)) return true;

                localRows[fila][num] = localCols[col][num] = localBoxes[boxIndex][num] = false;
                localGrid[fila][col] = 0;
            }
        }
        return false;
    };

    // Inicializar estructuras auxiliares
    std::vector<std::vector<bool>> rows(n, std::vector<bool>(n + 1, false));
    std::vector<std::vector<bool>> cols(n, std::vector<bool>(n + 1, false));
    std::vector<std::vector<bool>> boxes(n, std::vector<bool>(n + 1, false));

    // Llenar las estructuras auxiliares
    for (int fila = 0; fila < n; fila++) {
        for (int col = 0; col < n; col++) {
            if (grid[fila][col] != 0) {
                int num = grid[fila][col];
                int boxIndex = (fila / tamañomatriz) * tamañomatriz + (col / tamañomatriz);
                rows[fila][num] = cols[col][num] = boxes[boxIndex][num] = true;
            }
        }
    }

    // Detectar la primera celda vacía
    int iniciarfila = -1, startCol = -1;
    for (int i = 0; i < n && iniciarfila == -1; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) {
                iniciarfila = i;
                startCol = j;
                break;
            }
        }
    }

    if (iniciarfila == -1) return true; // Sudoku ya está resuelto

    // Paralelizar la búsqueda inicial
    #pragma omp parallel for shared(solucion)
    for (int num = 1; num <= n; num++) {
        if (!solucion && !rows[iniciarfila][num] && !cols[startCol][num] &&
            !boxes[(iniciarfila / tamañomatriz) * tamañomatriz + (startCol / tamañomatriz)][num]) {

            // Copia local para cada hilo
            std::vector<std::vector<int>> localGrid = grid;
            std::vector<std::vector<bool>> localRows = rows;
            std::vector<std::vector<bool>> localCols = cols;
            std::vector<std::vector<bool>> localBoxes = boxes;

            // Intentar colocar el número inicial
            localRows[iniciarfila][num] = localCols[startCol][num] =
                localBoxes[(iniciarfila / tamañomatriz) * tamañomatriz + (startCol / tamañomatriz)][num] = true;
            localGrid[iniciarfila][startCol] = num;

            // Ejecutar backtracking en la rama actual
            if (backtracking(localGrid, localRows, localCols, localBoxes, backtracking, iniciarfila, startCol + 1)) {
                #pragma omp atomic write
                solucion = true; // Usar atomic para evitar condiciones de carrera
                #pragma omp critical
                grid = localGrid; // Actualizar grid solo si se encontró solución
            }
        }
    }

    return solucion;
}