# Parking Lot Reference Design

## Responsibilities

| Type | Responsibility |
| --- | --- |
| `Vehicle` | Immutable vehicle identity and type |
| `ParkingSpot` | Compatibility and occupancy state for one spot |
| `PricingPolicy` | Replaceable fee calculation |
| `Ticket` | Stable record of an active parking session |
| `Receipt` | Completed session and final charge |
| `ParkingLot` | Allocation, active-session coordination, and synchronization |

## Ownership

`ParkingLot` owns spots by value and exclusively owns its pricing policy through `std::unique_ptr`. Tickets contain stable IDs rather than pointers into the spot container. Callers receive ticket and receipt values, so their lifetimes do not depend on the lot.

## Change isolation

`PricingPolicy` is a Strategy. It isolates fee-rule changes from parking allocation and session state. Spot compatibility remains close to `ParkingSpot`, which owns that rule for the current scope.

## Thread safety

A single mutex protects spots, active tickets, active license plates, and the ticket sequence. Parking and checkout update related state under one lock, preserving cross-container invariants. Finer-grained locking should be introduced only after measurement.

## Tradeoffs

The implementation linearly scans spots. That keeps the example interview-sized. At larger scale, available spot IDs could be indexed by type without changing the public interface.

The pricing policy is called while the mutex is held. The included policy is local and constant-time. A policy that performs I/O should receive an immutable session snapshot and run outside the critical section, with deliberate transaction semantics.
