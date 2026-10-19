# 5-Minute Demo Script

```text
CREATE CIRCLE c1 10 10 5
CREATE RECTANGLE r1 0 0 20 10 15
CREATE SPHERE s1 0 0 5 3
LIST
INFO c1
DISTANCE c1 r1
COLLISION c1 r1
MOVE c1 5 0
ROTATE r1 10
SCALE c1 1.5
UNDO
REDO
SAVE examples/demo_saved.geocad
CLEAR
LOAD examples/demo_saved.geocad
STATS
INTERSECT LINE 0 0 10 10 0 10 10 0
EXIT
```

### What to explain during the demo

1. `CREATE` goes through `ShapeFactory` and `AddShapeCommand`.
2. The scene stores polymorphic `Shape` objects in an `unordered_map`.
3. `COLLISION` delegates geometry-specific logic to `CollisionDetector`.
4. `UNDO` restores the previous scene snapshot through the Command Pattern.
5. `SAVE` serializes shapes into a readable text format.
