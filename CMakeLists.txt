cmake_minimum_required(VERSION 3.15)
project(paralelA)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Habilitar soporte para OpenMP
find_package(OpenMP REQUIRED)

# Agregar los archivos fuente al ejecutable
add_executable(paralelA
        main.cpp
        SudokuSolver.cpp
        FileOperations.cpp
        utils.cpp
)

# Verificar si OpenMP se encuentra y vincular correctamente
if(OpenMP_CXX_FOUND)
    target_link_libraries(paralelA PRIVATE OpenMP::OpenMP_CXX gomp)
else()
    message(FATAL_ERROR "OpenMP no encontrado. Asegúrese de que su compilador lo soporte.")
endif()

# Incluir directorios para las cabeceras de los archivos
target_include_directories(paralelA PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Directorio adicional de inclusión (si es necesario)
target_include_directories(paralelA PRIVATE "${CMAKE_SOURCE_DIR}/paralelA/include")

# Configuración especial para Windows (si es necesario)
if(WIN32)
    # Vincula OpenMP para Windows
    target_link_libraries(paralelA PRIVATE OpenMP::OpenMP_CXX)
endif()

# Comando para copiar el archivo JSON al directorio de compilación
set(SOURCE_JSON_PATH "${CMAKE_SOURCE_DIR}")
set(BUILD_JSON_PATH "${CMAKE_BINARY_DIR}")

add_custom_command(TARGET paralelA POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SOURCE_JSON_PATH}" "${BUILD_JSON_PATH}")

# Mensaje de verificación
message(STATUS "Copia de sudoku.json configurada de: ${SOURCE_JSON_PATH} a: ${BUILD_JSON_PATH}")
