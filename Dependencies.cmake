include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(cojoNES_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET Catch2::Catch2WithMain)
    CPMAddPackage("gh:catchorg/Catch2@3.8.0")
  endif()

  if(NOT TARGET fmtlib::fmtlib)
    CPMAddPackage("gh:fmtlib/fmt#11.1.4")
  endif()

  # SDL3 and Imgui must be in this order to ensure Imgui's SDL3 backend can see
  # the SDL3 header include paths. Imgui suggests that we build the specific
  # backends as part of our own project, but I think this is much cleaner.
  # Will revisit if we need multiple backends and causes things to get messy.
  if(NOT TARGET SDL3::SDL3)
    CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-3.2.8
        OPTIONS
        "BUILD_SHARED_LIBS ON"
        "SDL_DISABLE_INSTALL ON"
        "SDL_STATIC_PIC ON"
        "SDL_WERROR OFF"
        "SDL_LIBC ON")
  endif()

  if(NOT TARGET imgui)
    CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG v1.91.8)
  endif()

  add_library(imgui ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_demo.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp)

  target_include_directories(imgui SYSTEM PUBLIC ${SDL3_SOURCE_DIR}/include)
  include_directories(${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

  include_directories(${SDL3_INCLUDE_DIR})

  if(NOT TARGET spdlog::spdlog)
    CPMAddPackage(
      NAME
      spdlog
      VERSION
      1.15.1
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

endfunction()