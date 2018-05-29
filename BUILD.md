# Building Instruction

## Dependencies

The library depends on OpenCV >= 3.0

## Build

Just do the usual cmake and pass the path to the OpenCV install directory, where the `OpenCV-config.cmake` file can be found:

```bash
mkdir build && cd build
cmake .. -DOpenCV_DIR:PATH=/path/to/share/OpenCV/
make -j8
```

## Using the library as a third party

If you want to use the library as a third party you can install it somewhere, say `INSTALL_PATH`.
At cmake step you can pass `-DCMAKE_INSTALL_PREFIX`, e.g.

```bash
cmake .. -DOpenCV_DIR:PATH=/path/to/share/OpenCV/ -DCMAKE_INSTALL_PREFIX:PATH=INSTALL_PATH
make install
```

In order to include the library in your cmake project just add the dependency
```cmake
# Find the package from the chromakeyConfig.cmake
# in <prefix>/lib/cmake/chromakey/. Under the namespace chromakey::
# it exposes the target chromakey that allows you to compile
# and link with the library
find_package(chromakey CONFIG REQUIRED)
...
# suppose you want to try it out in a executable
add_executable(chromatest yourfile.cpp)
# add link to the library
target_link_libraries(chromatest PUBLIC chromakey::chromakey)
```

Then, in order to build just pass the location of `chromakeyConfig.cmake` from the cmake command line:
```bash
cmake .. -Dchromakey_DIR:PATH=INSTALL_PATH/lib/cmake/chromakey
```