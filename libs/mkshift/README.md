# MkShift

A simple C++ UI library that allows the creation of simple UIs using
XML and CSS.
Can currently be used as a library.
Support for use as a standalone application is planned in the future,
using LUA to fulfil the purpose of a JS-like scripting language. 

## Usage

```cpp
#include <mkshift.hpp>
#include <X11/Xlib.h>
#include <cairo-xlib.h>

int main(int argc, char** argv) {
    mkshift::Context mkctx;
    
    mkshift::Window()
}
```

## Automake

I have setup a `include.am` file that you can include that contains all the include flags for C/C++
It defines a variable `INCLUDEMKSHIFT` that you can add to you AM\_CFLAGS/AM\_CXXFLAGS
