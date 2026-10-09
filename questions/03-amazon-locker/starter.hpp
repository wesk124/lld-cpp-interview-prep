#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace amazon_locker {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

enum class Size { small, medium, large };
struct Slot {
    int id; Size size; bool occupied;
    Slot(int slot_id, Size slot_size) : id(slot_id), size(slot_size), occupied(false) {}
};
class AllocationPolicy {
public:
    virtual ~AllocationPolicy() {}
    virtual int choose(const std::vector<Slot>& slots, Size package_size) const = 0;
};
class SmallestFit : public AllocationPolicy {
public:
    int choose(const std::vector<Slot>& /*slots*/, Size /*package_size*/) const override {
        todo("choose smallest available compatible slot index, or -1");
    }
};
class Locker {
public:
    Locker(std::vector<Slot> /*slots*/, const AllocationPolicy& /*allocation*/) {
        todo("own slots and assignments; borrow the allocation Strategy");
    }
    int deposit(int /*package_id*/, Size /*size*/) { todo("allocate slot and issue integer code"); }
    int pickup(int /*code*/) { todo("invalidate code, release slot and return package ID"); }
    int available_slots() const { todo("count free slots"); }
};

} // namespace amazon_locker
} // namespace lld
