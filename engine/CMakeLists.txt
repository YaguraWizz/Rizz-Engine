cmake_minimum_required(VERSION 3.10)

# Устанавливаем имя библиотеки
project(Engine VERSION 1.0.0 LANGUAGES CXX)

# Рекурсивный сбор всех исходников и заголовков
file(GLOB_RECURSE ENGINE_SOURCES *.cpp *.h)

# Проверка наличия исходников
if(NOT ENGINE_SOURCES)
    message(FATAL_ERROR "No source files found in the 'engine/' directory.")
endif()


# Создаем статическую библиотеку для движка
add_library(Engine STATIC ${ENGINE_SOURCES})

# Указываем каталоги для заголовочных файлов
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Линкуем сторонние библиотеки
target_link_libraries(Engine PUBLIC third_party_libraries)

# Определяем платформу и передаем соответствующие макросы
if(WIN32)
    target_compile_definitions(Engine PRIVATE PLATFORM_WINDOWS)
    target_compile_definitions(Engine PRIVATE RZ_ENABLE_ASSERT)
elseif(APPLE)
    target_compile_definitions(Engine PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(Engine PRIVATE PLATFORM_UNIX)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Для precompiled headers (если используется поддержка CMake >= 3.16)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.16")
    target_precompile_headers(Engine PUBLIC hzpch.cpp)
endif()


# Устанавливаем виртуальные группы для Visual Studio с относительными путями
foreach(source_file IN LISTS ENGINE_SOURCES)
    # Получаем путь файла относительно текущей директории
    get_filename_component(file_dir ${source_file} DIRECTORY)
    # Обрезаем часть пути, чтобы он был относительно CMake текущей директории
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" file_dir ${file_dir})
    # Преобразуем путь к файлу в виртуальную группу (заменяем слеши)
    string(REPLACE "/" "\\" file_dir ${file_dir})
    # Применяем source_group
    source_group(${file_dir} FILES ${source_file})
endforeach()

message(STATUS "Engine library configured successfully.")
