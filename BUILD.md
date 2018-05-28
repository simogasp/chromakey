# Building Instruction

## Dependencies

The library depends on OpenCV > 3.0

## Build

Just do the usual cmake and pass the path to the OpenCV install directory, where the `OpenCV-config.cmake` file can be found:

```bash
mkdir build && cd build
cmake .. -DOpenCV_DIR=/path/to/share/OpenCV/
make -j8
```