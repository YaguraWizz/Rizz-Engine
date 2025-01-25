# CMakeLists.txt для сборки библиотеки glad

cmake_minimum_required(VERSION 3.10)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем имя статической библиотеки
add_library(glad STATIC)

# Указываем исходные файлы для библиотеки
target_sources(glad PRIVATE
    src/glad.c
)

# Указываем директорию с заголовочными файлами
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


