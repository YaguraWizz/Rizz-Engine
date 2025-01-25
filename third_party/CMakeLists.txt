cmake_minimum_required(VERSION 3.10)

# Создаем целевую библиотеку для всех зависимостей
add_library(third_party_libraries INTERFACE)

# Функция для добавления и настройки сторонней библиотеки
function(add_and_link_library library_dir target)
    # Проверяем существование директории с библиотекой
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${library_dir}")
        message(STATUS "Found library at: ${CMAKE_CURRENT_SOURCE_DIR}/${library_dir}")
    else()
        message(FATAL_ERROR "Directory not found at: ${CMAKE_CURRENT_SOURCE_DIR}/${library_dir}")
    endif()

    # Добавляем подкаталог с библиотекой
    add_subdirectory(${library_dir})
    message(STATUS "Successfully added ${library_dir} subdirectory")

    # Линкуем библиотеку с проектом
    target_link_libraries(${target} INTERFACE ${library_dir})
    message(STATUS "Successfully linked library: ${library_dir} to target ${target}")

    # Завершаем процесс
    message(STATUS "Library ${library_dir} setup completed successfully")
endfunction()

add_and_link_library("spdlog" "third_party_libraries")
add_and_link_library("glfw" "third_party_libraries")
add_and_link_library("glad" "third_party_libraries")

message(STATUS "All third-party libraries have been processed.")
