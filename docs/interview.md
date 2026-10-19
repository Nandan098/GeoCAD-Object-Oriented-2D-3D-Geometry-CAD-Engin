# Interview Preparation

## 30-second explanation

"GeoCAD is a C++17 mini CAD and computational-geometry engine. I designed it around a polymorphic shape hierarchy and separated geometry algorithms, scene management, commands and persistence. It supports transformations, intersections, collision detection, save/load and undo/redo, with automated tests for the core algorithms."

## Strong talking points

### Why C++?
Because the project exercises object-oriented design, value semantics, RAII, smart pointers, STL containers and explicit numerical algorithms in a language commonly used for performance-sensitive engineering software.

### Why an abstract Shape class?
It allows the scene and application layers to work with a common interface while each geometry type supplies its own mathematical implementation.

### Why `unordered_map` for the scene?
Object IDs are the natural key, and average O(1) lookup is useful for interactive commands such as `MOVE c1` and `INFO c1`.

### Why a command manager?
Undo/redo is an application-level concern. Keeping it out of `Shape` preserves the single responsibility of geometry objects.

### How do you handle floating point?
Geometric decisions use an epsilon rather than direct floating-point equality.

### How would you scale the engine?
I would add a spatial index such as an octree/BVH for broad-phase collision, reduce snapshot size with incremental change sets, and add a dedicated rendering/GUI layer without coupling it to the geometry core.

### What is the main limitation?
Some generic shape collisions currently use AABB as a conservative fallback rather than exact shape-specific algorithms. That is a deliberate extension point.
