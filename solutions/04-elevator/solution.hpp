#pragma once

#include <cstdlib>
#include <set>
#include <stdexcept>
#include <vector>

namespace lld {
namespace elevator {

enum class Direction { idle, up, down };

class Elevator {
public:
    Elevator(int top_floor, int initial_floor = 0)
        : top_(top_floor), floor_(initial_floor), direction_(Direction::idle), door_open_(false) {}

    void request_stop(int floor) {
        if (floor < 0 || floor > top_) throw std::out_of_range("invalid floor");
        if (floor != floor_ || !door_open_) stops_.insert(floor);
    }

    void step() {
        if (door_open_) { door_open_ = false; return; }
        if (stops_.erase(floor_) != 0) {
            door_open_ = true;
            if (stops_.empty()) direction_ = Direction::idle;
            return;
        }
        if (stops_.empty()) { direction_ = Direction::idle; return; }

        if (direction_ == Direction::idle) {
            int target = *stops_.begin();
            for (int stop : stops_)
                if (std::abs(stop - floor_) < std::abs(target - floor_)) target = stop;
            direction_ = target > floor_ ? Direction::up : Direction::down;
        } else if (direction_ == Direction::up && stops_.upper_bound(floor_) == stops_.end()) {
            direction_ = Direction::down;
        } else if (direction_ == Direction::down && stops_.lower_bound(floor_) == stops_.begin()) {
            direction_ = Direction::up;
        }

        floor_ += direction_ == Direction::up ? 1 : -1;
        if (stops_.erase(floor_) != 0) {
            door_open_ = true;
            if (stops_.empty()) direction_ = Direction::idle;
        }
    }

    int floor() const { return floor_; }
    bool door_open() const { return door_open_; }
    Direction direction() const { return direction_; }
    std::size_t pending() const { return stops_.size(); }

private:
    int top_;
    int floor_;
    Direction direction_;
    bool door_open_;
    std::set<int> stops_;
};

// Strategy: car selection varies independently of movement.
class DispatchPolicy {
public:
    virtual ~DispatchPolicy() {}
    virtual std::size_t choose(const std::vector<Elevator>& cars, int floor) const = 0;
};

class NearestCar : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Elevator>& cars, int floor) const override {
        std::size_t best = 0;
        for (std::size_t i = 1; i < cars.size(); ++i)
            if (std::abs(cars[i].floor() - floor) < std::abs(cars[best].floor() - floor)) best = i;
        return best;
    }
};

class LeastBusyCar : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Elevator>& cars, int /*floor*/) const override {
        std::size_t best = 0;
        for (std::size_t i = 1; i < cars.size(); ++i)
            if (cars[i].pending() < cars[best].pending()) best = i;
        return best;
    }
};

class ElevatorBank {
public:
    ElevatorBank(int top_floor, const std::vector<int>& initial_floors, const DispatchPolicy& policy)
        : policy_(policy) {
        if (initial_floors.empty()) throw std::invalid_argument("no cars");
        for (int floor : initial_floors) cars_.emplace_back(top_floor, floor);
    }

    std::size_t request(int floor) {
        std::size_t car = policy_.choose(cars_, floor);
        cars_.at(car).request_stop(floor);
        return car;
    }
    void select_floor(std::size_t car, int floor) { cars_.at(car).request_stop(floor); }
    void step_all() { for (auto& car : cars_) car.step(); }
    const Elevator& car(std::size_t index) const { return cars_.at(index); }

private:
    std::vector<Elevator> cars_;
    const DispatchPolicy& policy_; // Caller keeps this policy alive.
};

} // namespace elevator
} // namespace lld
