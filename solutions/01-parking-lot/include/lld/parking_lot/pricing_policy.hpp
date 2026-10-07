#pragma once

#include "lld/parking_lot/types.hpp"

#include <algorithm>
#include <chrono>
#include <limits>
#include <stdexcept>

namespace lld::parking_lot {

struct PricingQuote {
    int charged_hours;
    int fee_cents;
};

class PricingPolicy {
public:
    virtual ~PricingPolicy() = default;
    [[nodiscard]] virtual PricingQuote calculate(TimePoint entered_at,
                                                 TimePoint exited_at) const = 0;
};

class HourlyPricingPolicy final : public PricingPolicy {
public:
    explicit HourlyPricingPolicy(int cents_per_hour) : cents_per_hour_(cents_per_hour) {
        if (cents_per_hour <= 0) {
            throw std::invalid_argument("hourly rate must be positive");
        }
    }

    [[nodiscard]] PricingQuote calculate(TimePoint entered_at,
                                         TimePoint exited_at) const override {
        if (exited_at < entered_at) {
            throw std::invalid_argument("exit time cannot precede entry time");
        }

        using Rep = TimePoint::duration::rep;
        const auto entry_ticks = entered_at.time_since_epoch().count();
        const auto exit_ticks = exited_at.time_since_epoch().count();
        if (entry_ticks < 0 && exit_ticks > std::numeric_limits<Rep>::max() + entry_ticks) {
            throw std::overflow_error("session duration overflow");
        }
        const auto elapsed = exited_at - entered_at;
        const auto rounded = std::chrono::ceil<std::chrono::hours>(elapsed).count();
        if (rounded > std::numeric_limits<int>::max() / cents_per_hour_) {
            throw std::overflow_error("fee exceeds integer cents range");
        }
        const int hours = std::max(1, static_cast<int>(rounded));
        return PricingQuote{hours, hours * cents_per_hour_};
    }

private:
    int cents_per_hour_;
};

}  // namespace lld::parking_lot
