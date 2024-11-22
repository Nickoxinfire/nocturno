#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <vector>
#include <string>

std::vector<std::vector<int>> cargarsudoku(const std::string& filename);
void guardarsudoku(const std::string& filename, const std::vector<std::vector<int>>& grid, int n, double elapsedTime);

#endif
