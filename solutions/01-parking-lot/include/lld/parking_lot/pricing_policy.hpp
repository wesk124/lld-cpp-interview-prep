#pragma once

#include "lld/parking_lot/types.hpp"

#include <algorithm>
#include <chrono>
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

        const auto elapsed = exited_at - entered_at;
        const auto rounded = std::chrono::ceil<std::chrono::hours>(elapsed).count();
        const int hours = std::max(1, static_cast<int>(rounded));
        return PricingQuote{.charged_hours = hours, .fee_cents = hours * cents_per_hour_};
    }

private:
    int cents_per_hour_;
};

}  // namespace lld::parking_lot
