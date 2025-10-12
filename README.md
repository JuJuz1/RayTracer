# RayTracer

A learning project following Peter Shirley's book series [_Ray Tracing in One Weekend_](https://github.com/RayTracing/raytracing.github.io) (Version 4.0.2, 2025-04-25)

The goal for the first book is to create a C++ ray tracer which can create good-looking images

---

## Table of Contents

1. [Current features](#current-features)
2. [Example renders](#example-renders)
3. [Running Locally](#running-locally)
4. [Tools](#tools)

## Current features

- A brute-force raytracer in C++23
- Outputs images in **P3 PPM** format
- Some command line arguments (will probably add more)
- Progress indicators while rendering
- Only supports spheres as shapes at the moment
- Many randomized properties to provide unique renders each time
- Multithreading support for significantly faster renders

---

## Example renders

Image properties:

- width: 1200
- height: 675
- samples per pixel: 500
- max depth: 50
![An example of a final render](renders/image_final.png)

- samples per pixel: 100
![An example of a final render](renders/image_samples100.png)

## Running Locally

### Prerequisites

- A C++11 (or later) compatible compiler (GCC, MSVC or Clang)
- [CMake](https://cmake.org/) (version 3.1.0 or later recommended)

### Build Instructions

1. **Clone the repository**

```bash
git clone https://github.com/JuJuz1/RayTracer.git
cd RayTracer
```

2. **Build with CMake**

```bash
cmake -B $(your_build_directory_name) # e.g. cmake -B build
cmake --build $(your_build_directory_name)
# for optimized builds
cmake --build $(your_build_directory_name) --config Release
```

3. **Run the program**

You can specify the number of threads to use, default is 1. The default output file is image.ppm

```bash
./$(your_build_directory_name)/Release/inOneWeekend [num_threads] [output_filename.ppm]
```

There are many ways to view the created **P3 PPM** images. I found it easiest to use this website: [PPM Viewer](https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html)

## Tools

This project includes a copy of cpplint.py. See [License](LICENSE.cpplint)

To verify that the source files comply with most of the cpplint checks run:

```bash
./run_cpplint.sh
```
