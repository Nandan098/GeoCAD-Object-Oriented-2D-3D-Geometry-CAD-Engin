#pragma once

#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

namespace geocad_test {

inline int assertions = 0;
inline int failures = 0;

inline void record(bool condition, const char* expression, const char* file, int line, const std::string& detail = {})
{
    ++assertions;
    if (!condition) {
        ++failures;
        std::cerr << "  FAIL: " << expression << " at " << file << ':' << line;
        if (!detail.empty()) {
            std::cerr << " — " << detail;
        }
        std::cerr << '\n';
    }
}

inline void expectNear(double expected, double actual, double tolerance,
                       const char* expression, const char* file, int line)
{
    const bool condition = std::abs(expected - actual) <= tolerance;
    record(condition, expression, file, line,
           "expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual));
}

inline void expectThrows(const std::function<void()>& function,
                         const char* expression, const char* file, int line)
{
    bool thrown = false;
    try {
        function();
    } catch (...) {
        thrown = true;
    }
    record(thrown, expression, file, line);
}

inline int run(const std::string& name, const std::function<void()>& function)
{
    const int failuresBefore = failures;
    std::cout << "[TEST] " << name << '\n';
    try {
        function();
    } catch (const std::exception& exception) {
        ++failures;
        std::cerr << "  UNEXPECTED EXCEPTION: " << exception.what() << '\n';
    } catch (...) {
        ++failures;
        std::cerr << "  UNEXPECTED NON-STANDARD EXCEPTION\n";
    }
    return failures == failuresBefore ? 0 : 1;
}

} // namespace geocad_test

#define EXPECT_TRUE(condition) geocad_test::record((condition), #condition, __FILE__, __LINE__)
#define EXPECT_FALSE(condition) geocad_test::record(!(condition), "!" #condition, __FILE__, __LINE__)
#define EXPECT_EQ(expected, actual) geocad_test::record(((expected) == (actual)), #expected " == " #actual, __FILE__, __LINE__)
#define EXPECT_NE(expected, actual) geocad_test::record(((expected) != (actual)), #expected " != " #actual, __FILE__, __LINE__)
#define EXPECT_NEAR(expected, actual, tolerance) geocad_test::expectNear((expected), (actual), (tolerance), #expected " ~= " #actual, __FILE__, __LINE__)
#define EXPECT_THROW(statement) geocad_test::expectThrows([&]() { statement; }, "throws(" #statement ")", __FILE__, __LINE__)
