#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace elevator {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

enum class Direction { idle, up, down };
class Elevator {
public:
    Elevator(int /*top_floor*/, int /*initial_floor*/ = 0) { todo("initialize car state"); }
    void request_stop(int /*floor*/) { todo("validate floor and coalesce stop requests"); }
    void step() { todo("close doors, select direction, move one floor and service stops"); }
    int floor() const { todo("return current floor"); }
    bool door_open() const { todo("return door state"); }
    Direction direction() const { todo("return direction"); }
    std::size_t pending() const { todo("return pending stop count"); }
};
class DispatchPolicy {
public:
    virtual ~DispatchPolicy() {}
    virtual std::size_t choose(const std::vector<Elevator>& cars, int floor) const = 0;
};
class NearestCar : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Elevator>& /*cars*/, int /*floor*/) const override {
        todo("choose nearest car with first-index tie break");
    }
};
class LeastBusyCar : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Elevator>& /*cars*/, int /*floor*/) const override {
        todo("choose car with fewest pending stops");
    }
};
class ElevatorBank {
public:
    ElevatorBank(int /*top_floor*/, const std::vector<int>& /*initial_floors*/,
                 const DispatchPolicy& /*policy*/) { todo("own cars and borrow dispatch Strategy"); }
    std::size_t request(int /*floor*/) { todo("delegate car selection and enqueue stop"); }
    void select_floor(std::size_t /*car*/, int /*floor*/) { todo("enqueue an in-car selection"); }
    void step_all() { todo("advance every car"); }
    const Elevator& car(std::size_t /*index*/) const { todo("return the selected car"); }
};

} // namespace elevator
} // namespace lld
