Bit-Engine
===

Bit - A bit of help(A helpful multimedia framework for Windows and Linux)

Required libraries
---
 - X11 (Linux)
 - OpenGL
 - <del>OpenAL</del> ( Not yet )
 - <del>Freetype</del> ( Not yet )

Installation
---
```sh
git clone https://github.com/jimmiebergmann/Bit-Engine.git
```
**Windows:** Open and build the Visual Studio 2008 solution.
<br>**Linux:** Open and build the Code::Blocks 10 workspace.
<br>Or:
```sh
cd build/makefile
make
```

Usage
---
Do not use the dynamic builds. They are corrupt.
Add BIT_STATIC_LIB to your preprocessor if you are using the static libraries and<br>do not forget to link all the external libraries by yourself<br>if you are making your own project files for any OS.


Examples
---
 - [Bit-Engine-Examples](https://github.com/jimmiebergmann/Bit-Engine-Examples/ "Bit-Engine-Examples")
