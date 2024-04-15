# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(cool_sign_project VERSION 1.0 DESCRIPTION "Group project by Left 4 Read" LANGUAGES CXX)

# Compiler options (inherited by sub-folders)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

# export compile_commands.json for vim setup
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)

