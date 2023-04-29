# WMFConverter

A program to <b>convert</b> WMF files to PNG files and adjust <b>width</b> and <b>height</b> to pixels.

Usage: WMFConverter.exe target-folder width height

The converter recursively loops through the target-folder and converts all WMF files into PNG format.
The program creates a folder 'PNG' in which the new PNG files are stored in a similar folder-structure as they are found in the target-folder.
The WMF files will get resized to the given width and height.

Dependencies:
* <b>Windows API</b>
* <b>GDI+</b>

Tested on:
* OS: Windows 11 Pro x64
* C++ 20
* Compiler: Cygwin64 v3.4.3
* CMake Version 3.22