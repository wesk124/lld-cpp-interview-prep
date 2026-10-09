# Parking Lot: Interview Design

[Question](../../questions/01-parking-lot/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `ParkingSpot` | Compatibility and occupancy. |
| `Ticket` | Numeric ticket/spot IDs, plate and entry time. |
| `ParkingLot` | Choose a spot, track sessions and checkout. |
| `PricingPolicy` | Abstract fee calculation; HourlyPricing and FlatPricing are alternatives. |

park scans for a free compatible spot and creates a ticket. checkout finds the numeric ticket, calls pricing.fee(elapsed_minutes), frees its spot and erases the ticket.

## Ownership and scope

The lot owns spots and ticket values. It borrows const PricingPolicy&; the caller creates the policy before the lot and keeps it alive for the lot's lifetime.

Single-threaded, in-memory model. Time is supplied as nonnegative integer minutes. Construction supplies unique, initially free spot IDs and reasonable nonnegative prices/counts. There is no payment or persistence layer.

## Small usage example

Within the example's namespace:

```cpp
HourlyPricing pricing(500);
ParkingLot lot({ParkingSpot(1, SpotType::compact)}, pricing);
long long ticket = lot.park(Vehicle{"ABC123", VehicleType::car}, 0);
int cents = lot.checkout(ticket, 75); // 1000
```

Park is O(spots + active tickets); checkout scans O(spots), plus expected O(1) ticket lookup. These scans keep the interview implementation straightforward.

## Follow-up discussion

- Add a mutex around each complete park/checkout operation if concurrent entrances are required.
- Add a discount decorator, or extend the pricing input for time-of-day fees.

[Pattern map and catalog](../../docs/design-patterns.md)
