#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace lld::parking_lot::question {

// TODO: Define strongly typed vehicle and parking-spot categories.

struct Vehicle {
    // TODO: Add the minimum identity and classification fields.
};

struct Ticket {
    // TODO: Store stable identifiers and the entry timestamp.
};

struct Receipt {
    // TODO: Store the completed session and calculated fee.
};

class PricingPolicy {
public:
    virtual ~PricingPolicy() = default;

    // TODO: Define a const fee-calculation interface that accepts deterministic times.
};

class ParkingSpot {
public:
    // TODO: Add construction, compatibility, occupancy, and release behavior.

private:
    // TODO: Store spot identity, category, and optional occupancy.
};

class ParkingLot {
public:
    // TODO: Accept owned spots and an exclusively owned pricing strategy.

    // TODO: Define park and exit operations using value-oriented results.

private:
    // TODO: Add allocation behavior.
    // TODO: Add active-session state and explicitly protect its invariants.
};

}  // namespace lld::parking_lot::question
