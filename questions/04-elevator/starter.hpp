#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace lld::elevator {
enum class Direction { idle, up, down };
enum class Door { closed, open };
struct Snapshot { int floor; Direction direction; Door door; std::vector<int> pending; };
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class Elevator {
public:
    explicit Elevator(int /*top_floor*/, int /*initial_floor*/ = 0) {
        todo("validate floor bounds and initialize an idle car");
    }
    void request_stop(int /*floor*/) { todo("validate and coalesce stop requests"); }
    Snapshot snapshot() const { todo("return a value snapshot"); }
    Snapshot step() {
        todo("close doors, move at most one floor, and serve requests with LOOK ordering");
    }
private:
    // TODO: Track floor, direction, door, and pending stops without global state.
};
class DispatchPolicy {
public:
    virtual ~DispatchPolicy() = default;
    virtual std::size_t choose(const std::vector<Snapshot>& cars, int floor, Direction direction) const = 0;
};
class NearestCarPolicy final : public DispatchPolicy {
public:
    std::size_t choose(const std::vector<Snapshot>& /*cars*/, int /*floor*/,
                       Direction /*direction*/) const override {
        todo("choose nearest car with index-based tie breaking");
    }
};
class ElevatorBank {
public:
    ElevatorBank(int /*top_floor*/, const std::vector<int>& /*initial_floors*/,
                 std::unique_ptr<DispatchPolicy> /*policy*/) {
        todo("own cars and a replaceable dispatch strategy");
    }
    std::size_t request(int /*floor*/, Direction /*direction*/) {
        todo("validate a hall call and route it to one car");
    }
    void select_floor(std::size_t /*car*/, int /*floor*/) { todo("handle a car's internal button"); }
    std::vector<Snapshot> step_all() { todo("serialize one simulation tick"); }
    std::vector<Snapshot> snapshots() const { todo("return synchronized car snapshots"); }
private:
    // TODO: Put synchronization at the bank boundary; never move with open doors.
};
}  // namespace lld::elevator
