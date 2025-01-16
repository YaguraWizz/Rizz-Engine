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

message("------------- Project files generation for Engine -------------")

# Собираем исходники и заголовочные файлы движка
file(GLOB_RECURSE ENGINE_SOURCES engine/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS engine/*.h)

# Проверка наличия исходников для движка
if(NOT ENGINE_SOURCES)
    message(FATAL_ERROR "No source files found in the 'engine/' directory.")
endif()

# Создаем статическую библиотеку для движка
add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Указываем каталоги для заголовочных файлов движка
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine)

# Линкуем сторонние библиотеки с движком
target_link_libraries(Engine PUBLIC third_party_libraries)

# Определяем платформу и передаем соответствующие макросы
if(WIN32)
    # Для Windows
    target_compile_definitions(Engine PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    # Для macOS
    target_compile_definitions(Engine PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    # Для Linux или других Unix-подобных систем
    target_compile_definitions(Engine PRIVATE PLATFORM_UNIX)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()


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
