cmake_minimum_required(VERSION 3.10)

# --------------------------
# 1. Установка параметров проекта
# --------------------------

# Устанавливаем имя проекта и стандарт языка
set(PROJECT_NAME Editor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


message("------------- Project files generation for third_party libs -------------")

# --------------------------
# 2. Добавление сторонних библиотек
# --------------------------

# Включаем папку third_party для сторонних библиотек
add_subdirectory(third_party)

# --------------------------
# 3. Сборка игрового движка (Engine)
# --------------------------

# Добавляем движок
add_subdirectory(engine)

# --------------------------
# 4. Сборка редактора (Editor)
# --------------------------

message("------------- Project files generation for ${PROJECT_NAME} -------------")

# Собираем исходники и заголовочные файлы редактора
file(GLOB_RECURSE EDITOR_SOURCES editor/*.cpp)
file(GLOB_RECURSE EDITOR_HEADERS editor/*.h)

# Проверка наличия исходников для редактора
if(NOT EDITOR_SOURCES)
    message(FATAL_ERROR "No source files found in the 'editor/' directory.")
endif()

# Создаем исполняемый файл для редактора
add_executable(${PROJECT_NAME} ${EDITOR_SOURCES} ${EDITOR_HEADERS})

# Линкуем движок с редактором
target_link_libraries(${PROJECT_NAME} PUBLIC Engine)

# Указываем каталоги для заголовочных файлов редактора
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine)


# --------------------------
# 5. Вывод информации о конфигурации проекта
# --------------------------

# Выводим информацию о успешной конфигурации проекта
message(STATUS "Project ${PROJECT_NAME} is configured successfully in ${CMAKE_BUILD_TYPE} mode.")
