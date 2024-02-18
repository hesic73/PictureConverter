## PictureConverter

[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/hesic73/PictureConverter/master/LICENSE)

**PictureConverter** is a simple, toy project developed with Qt 5.15.2. It provides a basic GUI for users to convert image files between different formats.


https://github.com/hesic73/PictureConverter/assets/59533230/5941e878-9ef6-482c-8b7f-ef45b4a0ce16


## Usage

- Drag and drop images or use the file dialog to add images.
- View basic image information before conversion.
- Select an output directory for converted images.
- Choose the desired output format for the conversion.

## Build from source

Before starting the build process, confirm that Qt and CMake are properly installed on your system. Open `CMakeLists.txt` within Qt Creator as a Qt project for building. Unfortunately, when attempting to compile via the command line using the following commands, I encountered linker errors due to unresolved symbols, and the root cause remains unidentified:

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
