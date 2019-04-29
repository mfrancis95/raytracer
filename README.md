# Ray Tracer

## Requirements

* g++
* make
* OpenCL
* OpenGL 4.5
* SDL2

## Compiling

In the directory of the repository, simply run `make` to build all of the executables.

To compile with extra performance for the software renderer, run `make FLAGS=-O3`.

After compiling, the executables `compute_renderer`, `fragment_renderer` and `software_renderer` will have been built into the same directory.

## Running

The programs can be executed by running `./compute_renderer`, `./fragment_renderer` and `./software_renderer`.

It's recommended to build on a Linux machine for now, since this has not been tested on other platforms yet.
