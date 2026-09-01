#pragma once

#include <chrono>
#include <string>

namespace lld::parking_lot {

using TimePoint = std::chrono::system_clock::time_point;

enum class VehicleType { motorcycle, car, truck };
enum class SpotType { motorcycle, compact, large };

struct Vehicle {
    std::string license_plate;
    VehicleType type;
};

struct Ticket {
    std::string id;
    std::string license_plate;
    std::string spot_id;
    TimePoint entered_at;
};

struct Receipt {
    std::string ticket_id;
    std::string license_plate;
    std::string spot_id;
    TimePoint entered_at;
    TimePoint exited_at;
    int charged_hours;
    int fee_cents;
};

}  // namespace lld::parking_lot
