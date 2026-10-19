# Algorithms

## Vector operations

Dot product and cross product are implemented directly from their mathematical definitions.

## Line intersection

The engine uses the determinant-based segment intersection formula. Parallel and coincident cases are handled explicitly.

## Line-circle intersection

The segment is parameterized as `P(t) = P0 + t(D)`. Substitution into the circle equation produces a quadratic. Real roots in `[0,1]` correspond to segment intersections.

## Circle-circle intersection

The engine derives the distance between centers, the distance from the first center to the chord midpoint, and the perpendicular offset to obtain zero, one, or two intersection points.

## Point in polygon

Ray casting toggles an `inside` flag whenever a horizontal ray crosses a polygon edge.

## Rectangle collision

Two rotated rectangles are tested with the Separating Axis Theorem (SAT). The candidate axes are the normals of the rectangle edges.

## AABB broad phase

For general shapes, the engine uses axis-aligned bounding boxes as a conservative collision test. This is intentionally simple and can be replaced with an exact pairwise strategy or spatial tree later.
