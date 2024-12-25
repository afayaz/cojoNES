add_executable(cojoNES main.cpp Cartridge.cpp CPU.cpp ROM.cpp System.cpp)
target_link_libraries(cojoNES)
target_link_system_libraries(cojoNES PRIVATE fmt::fmt imgui SDL3::SDL3 spdlog::spdlog)
