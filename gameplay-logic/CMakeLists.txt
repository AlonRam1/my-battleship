add_library(gameplay_logic board.cpp player.cpp ship.cpp shipnode.cpp gameplay.cpp)

target_include_directories(gameplay_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
