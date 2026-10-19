# Complexity Analysis

Let `n` be the number of objects in a scene and `v` the number of vertices in a polygon.

| Operation | Complexity | Notes |
|---|---:|---|
| Scene ID lookup | O(1) average | `unordered_map` |
| Scene insertion | O(1) average | Hash-table insertion |
| Scene deletion | O(1) average | Hash-table erase |
| List all objects | O(n log n) | IDs are sorted for deterministic output |
| Circle-circle intersection | O(1) | Constant-size arithmetic |
| Line-circle intersection | O(1) | Quadratic with constant degree |
| Point-in-polygon | O(v) | Ray-casting through all edges |
| Polygon area | O(v) | Shoelace formula |
| Rectangle SAT collision | O(1) | Fixed number of axes/vertices |
| Generic pair collision | Depends on shapes | AABB fallback is O(1) for bounding boxes |
| Undo | O(n) | Scene snapshot restoration |
| Redo | O(n) | Scene snapshot restoration |
| Save | O(n + total geometry size) | One serialization line per object |
| Load | O(n + total geometry size) | Parsing and construction |

## Why snapshot-based undo?

A command can store only the inverse operation, but that requires each command to perfectly reverse every possible future state. The snapshot approach is simpler and robust for a small educational engine. A larger CAD product would likely use object-level change sets or a transaction/history system to reduce memory usage.
