add_executable(cojoNES main.cpp Cartridge.cpp CPU.cpp ROM.cpp System.cpp)
target_link_libraries(
  cojoNES)
