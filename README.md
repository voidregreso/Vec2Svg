# Vector2Svg

Vector2Svg is a utility for converting Android Vector Drawable XML resource files to SVG (Scalable Vector Graphics) format.

## Description

Android VectorDrawable is a powerful drawable type introduced in Android, which allows for defining complex vector graphics in XML files. However, these files are not directly viewable or editable in many popular vector graphics software like Adobe Illustrator.

Vector2Svg solves this problem by converting Vector Drawable XML files into SVG files, which are widely supported by many graphics software. This utility is especially useful for designers and developers who want to view or edit this vector image using their preferred graphics software.

## Usage

Each argument to the utility should be a path to a Vector Drawable XML file. A new SVG file will be created in the same directory as the input file, with the same name but a ".svg" extension.

Here is an example of how to use the utility:

```bash
./vec2svg path/to/vectorDrawable1.xml path/to/vectorDrawable2.xml
```

In this example, two SVG files will be created: `path/to/vectorDrawable1.svg` and `path/to/vectorDrawable2.svg`.

## Building

This utility is written in C++ and uses the tinyxml2 library for parsing and creating XML files. You can build it using CMake:

```bash
mkdir build
cd build
cmake ..
make
```
For the MSVC compiler, I already have the compiled tinyxml2 library pre-built. At this point you can generate the vs solution directly via cmake and compile it with Visual Studio. 

If your compiler is not MSVC, or the OS platform is not Windows x64 at all, then please make sure the tinyxml2 library is installed and discoverable by CMake.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
