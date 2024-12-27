add_executable(cojoNES_tests test.cpp ../source/Cartridge.cpp ../source/CPU.cpp ../source/ROM.cpp ../source/System.cpp)
target_include_directories(cojoNES_tests PRIVATE ../source)
target_link_libraries(cojoNES_tests PRIVATE Catch2::Catch2WithMain)
target_link_system_libraries(cojoNES_tests PRIVATE fmt::fmt spdlog::spdlog)
