# Testing

GeoCAD includes a self-contained test runner so the repository can build without downloading a testing framework.

The test suite covers:

- 2D and 3D distance
- Vector dot/cross products
- Angles
- Point-to-line distance
- Area/perimeter/volume
- Transformations
- Intersections
- Point-in-polygon
- Collision detection
- Scene insertion/deletion
- Duplicate ID validation
- Undo/redo
- Serialization round-trips
- Invalid model rejection

Run:

```bash
cmake -S . -B build -DGEOCAD_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The executable test runner also prints individual test groups and a final assertion/failure count.
