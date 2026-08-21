cmake_minimum_required(VERSION 3.20)

project(Battleship)

add_subdirectory(gameplay-logic)
add_subdirectory(server)
add_subdirectory(client)

target_include_directories(gameplay_logic PUBLIC ${CMAKE_SOURCE_DIR}/common)
target_include_directories(my-game-server PRIVATE ${CMAKE_SOURCE_DIR}/common)
