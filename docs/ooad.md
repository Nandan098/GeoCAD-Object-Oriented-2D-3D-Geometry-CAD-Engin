# OOAD Notes

## Core abstractions

`Shape` is the primary abstraction. It expresses operations common to all modeled objects, such as area, bounding box, transformation, cloning and serialization.

Concrete classes specialize the implementation:

```text
Shape
├── Circle
├── Rectangle
├── Triangle
├── Polygon
├── Sphere
├── Cube
└── Cylinder
```

## Encapsulation

Shape state is private. Clients interact through focused methods such as `radius()`, `translate()`, `scale()` and `boundingBox()`.

## Polymorphism

A `Scene` stores `std::unique_ptr<Shape>`. Code operating on a scene can call `shape->area()` without knowing the concrete type.

## Composition

Complex subsystems are composed instead of inheriting from unrelated concepts. For example, a `Scene` owns shape objects while `CommandManager` owns command objects.

## Design principle

The project follows a simple rule: put a responsibility in the narrowest module that can own it without creating unnecessary coupling.
