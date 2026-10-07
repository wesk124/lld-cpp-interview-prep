#pragma once

#include <cstdlib>
#include <limits>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace lld::elevator {

enum class Direction { idle, up, down };
enum class Door { closed, open };
struct Snapshot {
    int floor;
    Direction direction;
    Door door;
    std::vector<int> pending;
};

// A discrete simulation; a bank serializes access to its owned cars.
class Elevator {
public:
    explicit Elevator(int top_floor, int initial_floor = 0)
        : top_floor_(top_floor), floor_(initial_floor) {
        if (top_floor < 1 || initial_floor < 0 || initial_floor > top_floor) {
            throw std::invalid_argument("invalid elevator floors");
        }
    }

    void request_stop(int floor) {
        if (floor < 0 || floor > top_floor_) { throw std::out_of_range("invalid floor"); }
        if (floor == floor_ && door_ == Door::open) { return; }
        stops_.insert(floor);  // Repeated requests are coalesced.
    }

    Snapshot snapshot() const {
        return Snapshot{floor_, direction_, door_, std::vector<int>(stops_.begin(), stops_.end())};
    }

    Snapshot step() {
        if (door_ == Door::open) {
            door_ = Door::closed;  // Closing consumes a tick; do not move yet.
            return snapshot();
        }
        if (stops_.erase(floor_) != 0) {
            door_ = Door::open;
            if (stops_.empty()) { direction_ = Direction::idle; }
            return snapshot();
        }
        if (stops_.empty()) {
            direction_ = Direction::idle;
            return snapshot();
        }
        if (direction_ == Direction::idle) {
            int target = *stops_.begin();
            for (int stop : stops_) {
                if (std::abs(stop - floor_) < std::abs(target - floor_)) { target = stop; }
            }
            direction_ = target > floor_ ? Direction::up : Direction::down;
        } else if (direction_ == Direction::up && stops_.upper_bound(floor_) == stops_.end()) {
            direction_ = Direction::down;
        } else if (direction_ == Direction::down && stops_.lower_bound(floor_) == stops_.begin()) {
            direction_ = Direction::up;
        }
        floor_ += direction_ == Direction::up ? 1 : -1;
        if (stops_.erase(floor_) != 0) {
            door_ = Door::open;
            if (stops_.empty()) { direction_ = Direction::idle; }
        }
        return snapshot();
    }

private:
    int top_floor_;
    int floor_;
    Direction direction_{Direction::idle};
    Door door_{Door::closed};
    std::set<int> stops_;
};

class DispatchPolicy {
public:
    virtual ~DispatchPolicy() = default;
    virtual std::size_t choose(const std::vector<Snapshot>& cars, int floor,
                               Direction direction) const = 0;
};

// Deliberately simple: nearest car, then smallest car index; hall direction
// is exposed for richer policies but does not influence this policy.
class NearestCarPolicy final : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Snapshot>& cars, int floor,
                       Direction /*direction*/) const override {
        if (cars.empty()) { throw std::invalid_argument("no cars"); }
        std::size_t best = 0;
        for (std::size_t i = 1; i < cars.size(); ++i) {
            if (std::abs(cars[i].floor - floor) < std::abs(cars[best].floor - floor)) { best = i; }
        }
        return best;
    }
};

class ElevatorBank {
public:
    ElevatorBank(int top_floor, const std::vector<int>& initial_floors,
                 std::unique_ptr<DispatchPolicy> policy)
        : top_floor_(top_floor), policy_(std::move(policy)) {
        if (!policy_ || initial_floors.empty()) { throw std::invalid_argument("cars and policy required"); }
        for (int floor : initial_floors) { cars_.emplace_back(top_floor, floor); }
    }

    std::size_t request(int floor, Direction direction) {
        if (floor < 0 || floor > top_floor_ || direction == Direction::idle ||
            (direction != Direction::up && direction != Direction::down) ||
            (floor == 0 && direction == Direction::down) ||
            (floor == top_floor_ && direction == Direction::up)) {
            throw std::invalid_argument("invalid hall call");
        }
        std::lock_guard<std::mutex> lock(mutex_);
        const std::size_t car = policy_->choose(snapshots_unlocked(), floor, direction);
        if (car >= cars_.size()) { throw std::logic_error("dispatcher returned invalid car"); }
        cars_[car].request_stop(floor);
        return car;
    }

    void select_floor(std::size_t car, int floor) {
        std::lock_guard<std::mutex> lock(mutex_);
        cars_.at(car).request_stop(floor);
    }

    std::vector<Snapshot> step_all() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& car : cars_) { car.step(); }
        return snapshots_unlocked();
    }

    std::vector<Snapshot> snapshots() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return snapshots_unlocked();
    }

private:
    std::vector<Snapshot> snapshots_unlocked() const {
        std::vector<Snapshot> result;
        for (const auto& car : cars_) { result.push_back(car.snapshot()); }
        return result;
    }
    int top_floor_;
    mutable std::mutex mutex_;
    std::vector<Elevator> cars_;
    std::unique_ptr<DispatchPolicy> policy_;
};

}  // namespace lld::elevator
