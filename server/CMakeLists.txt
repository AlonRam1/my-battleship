add_executable(my-game-server my-game-server.cpp server-socket.cpp)

target_link_libraries(my-game-server PRIVATE gameplay_logic)

target_include_directories(my-game-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


