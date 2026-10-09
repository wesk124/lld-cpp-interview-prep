#pragma once

#include <memory>
#include <stdexcept>
#include <utility>

namespace lld {

// Small optional-value helper for these examples. A present value is separately
// owned; copies duplicate it, moves transfer it, and reset releases it.
template <typename T>
class Optional {
public:
    Optional() noexcept = default;
    Optional(const T& value) : value_(new T(value)) {}
    Optional(T&& value) : value_(new T(std::move(value))) {}
    Optional(const Optional& other)
        : value_(other.value_ ? new T(*other.value_) : nullptr) {}
    Optional(Optional&&) noexcept = default;

    Optional& operator=(Optional other) noexcept {
        value_.swap(other.value_);
        return *this;
    }

    explicit operator bool() const noexcept { return has_value(); }
    bool has_value() const noexcept { return static_cast<bool>(value_); }

    T& value() {
        if (!value_) { throw std::logic_error("empty optional value"); }
        return *value_;
    }
    const T& value() const {
        if (!value_) { throw std::logic_error("empty optional value"); }
        return *value_;
    }
    T& operator*() { return value(); }
    const T& operator*() const { return value(); }
    T* operator->() { return &value(); }
    const T* operator->() const { return &value(); }

    void reset() noexcept { value_.reset(); }

private:
    std::unique_ptr<T> value_;
};

}  // namespace lld
