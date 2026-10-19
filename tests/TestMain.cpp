#include <iostream>

void runGeometryTests();
void runShapeTests();
void runIntersectionTests();
void runSceneTests();
void runCollisionTests();
void runCommandTests();
void runSerializationTests();

#include "TestFramework.h"

int main()
{
    runGeometryTests();
    runShapeTests();
    runIntersectionTests();
    runCollisionTests();
    runSceneTests();
    runCommandTests();
    runSerializationTests();

    std::cout << "\nAssertions: " << geocad_test::assertions
              << ", Failures: " << geocad_test::failures << '\n';
    return geocad_test::failures == 0 ? 0 : 1;
}
