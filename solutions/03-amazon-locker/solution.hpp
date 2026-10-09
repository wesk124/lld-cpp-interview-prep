#pragma once

#include <map>
#include <utility>
#include <vector>

namespace lld {
namespace amazon_locker {

enum class Size { small, medium, large };

struct Slot {
    int id;
    Size size;
    bool occupied;
    Slot(int slot_id, Size slot_size) : id(slot_id), size(slot_size), occupied(false) {}
};

// Strategy: selection is separate from deposit/pickup coordination.
class AllocationPolicy {
public:
    virtual ~AllocationPolicy() {}
    virtual int choose(const std::vector<Slot>& slots, Size package_size) const = 0;
};

class SmallestFit : public AllocationPolicy {
public:
    int choose(const std::vector<Slot>& slots, Size package_size) const override {
        int best = -1;
        for (std::size_t i = 0; i < slots.size(); ++i)
            if (!slots[i].occupied && slots[i].size >= package_size &&
                (best == -1 || slots[i].size < slots[static_cast<std::size_t>(best)].size))
                best = static_cast<int>(i);
        return best;
    }
};

class Locker {
public:
    Locker(std::vector<Slot> slots, const AllocationPolicy& allocation)
        : slots_(std::move(slots)), allocation_(allocation), next_code_(1) {}

    int deposit(int package_id, Size size) {
        if (package_id < 0) return -1;
        for (const auto& entry : assignments_)
            if (entry.second.package_id == package_id) return -1;
        int index = allocation_.choose(slots_, size);
        if (index < 0 || index >= static_cast<int>(slots_.size())) return -1;
        int code = next_code_++;
        assignments_.emplace(code, Assignment{package_id, index});
        slots_[static_cast<std::size_t>(index)].occupied = true;
        return code;
    }

    int pickup(int code) {
        auto assignment = assignments_.find(code);
        if (assignment == assignments_.end()) return -1;
        int package = assignment->second.package_id;
        slots_[static_cast<std::size_t>(assignment->second.slot_index)].occupied = false;
        assignments_.erase(assignment);
        return package;
    }

    int available_slots() const {
        int count = 0;
        for (const auto& slot : slots_) if (!slot.occupied) ++count;
        return count;
    }

private:
    struct Assignment { int package_id; int slot_index; };
    std::vector<Slot> slots_;
    const AllocationPolicy& allocation_; // Caller keeps this policy alive.
    std::map<int, Assignment> assignments_;
    int next_code_; // Demonstration codes, not production authentication.
};

} // namespace amazon_locker
} // namespace lld
