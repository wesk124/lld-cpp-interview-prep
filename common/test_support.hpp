#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace test {

inline void check(bool condition, const char* expression, const char* file, int line) {
    if (!condition) {
        throw std::runtime_error(std::string(file) + ":" + std::to_string(line) +
                                 ": check failed: " + expression);
    }
}

template <typename Exception, typename Function>
void expect_throw(Function&& function, const char* file, int line) {
    try {
        std::forward<Function>(function)();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(std::string(file) + ":" + std::to_string(line) +
                             ": expected exception was not thrown");
}

class Suite {
public:
    template <typename Function>
    void run(const std::string& name, Function&& function) {
        ++total_;
        try {
            std::forward<Function>(function)();
            std::cout << "PASS " << name << '\n';
        } catch (const std::exception& error) {
            ++failures_;
            std::cerr << "FAIL " << name << ": " << error.what() << '\n';
        } catch (...) {
            ++failures_;
            std::cerr << "FAIL " << name << ": unknown exception\n";
        }
    }

    int finish() const {
        std::cout << total_ - failures_ << "/" << total_ << " tests passed\n";
        return failures_ == 0 ? 0 : 1;
    }

private:
    int total_{0};
    int failures_{0};
};

}  // namespace test

#define CHECK(expression) test::check(static_cast<bool>(expression), #expression, __FILE__, __LINE__)
#define EXPECT_THROW(exception, expression) \
    test::expect_throw<exception>([&] { static_cast<void>(expression); }, __FILE__, __LINE__)
