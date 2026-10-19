# GeoCAD

GeoCAD is an educational, interview-ready mini CAD and computational-geometry engine written in modern C++17. It focuses on the software-engineering foundations used by larger geometry and virtual-design systems: object-oriented design, reusable geometry algorithms, transformations, collision detection, scene management, undo/redo, serialization, testing, and a command-line interface.

## Why this project?

The design intentionally emphasizes:

- C++17 and the STL
- OOP and OOAD
- Computational geometry
- Data structures and algorithms
- Numerical precision
- Design patterns
- Unit testing
- Clean separation of responsibilities

## Features

### Geometry
- 2D: Point, Vector, Line, Circle, Rectangle, Triangle, Polygon
- 3D: Point, Vector, Matrix, Bounding Box, Sphere, Cube, Cylinder
- Distance, angles, area, perimeter, volume
- Translation, rotation, scaling
- Line/circle/circle intersection algorithms
- Point-in-polygon test
- 2D and 3D collision detection

### Application
- Scene object registry
- Shape factory
- Undo/redo using Command Pattern
- Save/load custom `.geocad` format
- Robust CLI validation
- Logging
- Unit tests with a self-contained test runner

## Repository structure

```text
GeoCAD/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/geocad/...
├── src/...
├── tests/...
├── docs/...
└── examples/...
```

## Requirements

- C++17 compiler
- CMake 3.16+
- Git
- No external test dependency is required; the repository includes a small self-contained test runner.

## Build

```bash
cmake -S . -B build -DGEOCAD_BUILD_TESTS=ON
cmake --build build
```

Run the application:

```bash
./build/geocad
```

On Windows with a Visual Studio generator, the executable will be under the generated configuration directory.

## Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Example session

```text
GeoCAD > CREATE CIRCLE c1 10 10 5
Created CIRCLE 'c1'.

GeoCAD > CREATE RECTANGLE r1 0 0 20 10 0
Created RECTANGLE 'r1'.

GeoCAD > LIST

GeoCAD > INFO c1

GeoCAD > COLLISION c1 r1

GeoCAD > MOVE c1 5 0

GeoCAD > UNDO

GeoCAD > REDO

GeoCAD > SAVE examples/my_scene.geocad

GeoCAD > EXIT
```

## Design highlights

The engine uses an abstract `Shape` interface for polymorphic operations. `ShapeFactory` centralizes object creation. `CommandManager` owns command histories while concrete shape commands provide undo/redo behavior through scene snapshots. Geometry algorithms live in dedicated engine components rather than in the CLI.

## Floating-point precision

Geometry decisions use a shared epsilon-based comparison instead of direct equality. This reduces failures caused by tiny floating-point rounding differences.

## Future extensions

A GUI/OpenGL front-end, mesh support, spatial indexes such as an octree, parametric constraints, STL/OBJ export, and more specialized collision strategies can be added without replacing the core engine.
