## cojoNES

A NES emulator[^1], made for fun.

Ever since I used my first emulator as a young lad[^2], I was fascinated by it and wanted to learn how it worked. I understood the basic concept, but always wanted to dig deeper.

This is a personal learning project, and will never be of any practical use to anyone.

### Building

You will need CMake 3.21+ and a C++20 compliant compiler.

It is known to build and run on Windows 10 using Visual Studio 2022, as well as Fedora Linux using GCC 10.

The CMake files are based on [CMake Template](https://github.com/cpp-best-practices/cmake_template) by [Jason Turner](https://github.com/lefticus).

#### Dependencies

All dependencies are handled by [CPM](https://github.com/cpm-cmake/CPM.cmake), which should download and build the appropriate version from GitHub.

* [SDL3](https://github.com/libsdl-org/SDL): Interface with the underlying system for input, rendering and window handling.
	* Built as a static lib, although I plan to change that because the authors prefer that applications link to it dynamically[^3].
* [Dear Imgui](https://github.com/ocornut/imgui): Used for all debug visualisation UI.
	* Doesn't provide it's own `CMakeLists.txt`, so I've bodged something together myself.
	* Built as a separate lib (including the SDL3 backend) rather than following their recommendation to add its source to the main **cojoNES** target.
* [fmtlib](https://github.com/fmtlib/fmt): String formatting, more up to date and widely available than C++ `<format>`[^4].
* [spdlog](https://github.com/gabime/spdlog): Used for logging to the console.
* [Catch2](https://github.com/catchorg/Catch2): Unit testing framework used in **cojoNES_tests**.

### Resources

* [Nesdev wiki](https://www.nesdev.org/wiki/Nesdev_Wiki), contains tons of valuable documentation on writing your own NES programs as well as the internals of the hardware.
* [NES Emulator From Scratch tutorial series](https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf) by [javidx9 aka OneLoneCoder](https://github.com/OneLoneCoder). His entire YouTube channel is brilliant.
* [Jnes](https://www.jabosoft.com/jnes) by [Jabo](https://github.com/jabo2), the NES emulator that inspired me. It's lightweight, fast and the user interface is well designed.

[^1]: I pronounce "NES" as "nessss", that's how everyone said it in ol' Blighty.
[^2]: Some time in 2000, I downloaded an early pre-1.0 version of Jnes, with the much better (IMO) purple icon.
[^3]: SDL2 added a cool trick to allow overriding the SDL library with a different version at runtime, _even when statically linked_, by adding another layer of indirection[^5]. Nonetheless, being able to just replace a `.dll` or avoiding code bloat by linking to a shared `.so` is just nicer.
[^4]: Dear Imgui uses C style formatting as an intentional design choice to avoid external dependencies. After using `fmtlib`, it seems so antiquated. Having to switch between the two styles drives me mad, though.
[^5]: See [this old Google+ post by Ryan C. Gordon](https://web.archive.org/web/20150521170102/https://plus.google.com/+RyanGordon/posts/TB8UfnDYu4U).
