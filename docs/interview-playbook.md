# 45–60-Minute OOP Interview Playbook

## 1. Clarify the small workflow — 5 minutes

For Parking Lot, clarify vehicle compatibility, allocation, pricing and checkout. State whether the base model is single-threaded and in memory. Record more elaborate features as possible follow-ups.

## 2. Model responsibilities and the pattern — 10 minutes

Name a few objects and one invariant per important state transition. For example:

- ParkingSpot manages compatibility and occupancy.
- ParkingLot coordinates numeric tickets and spot assignment.
- PricingPolicy varies fee calculation: Strategy.

The interface isolates fee rules from allocation. HourlyPricing and FlatPricing demonstrate the substitution.

## 3. Implement one complete workflow — 25 minutes

Write park and checkout end-to-end. Use direct data structures and numeric IDs for identification. The reference examples use ordinary C++11 features and a small public API.

Explain ownership as you code: the lot owns its spots and tickets but borrows its pricing policy. The policy outlives the lot. Filesystem children have a different lifetime: directories exclusively own them through unique_ptr.

## 4. Test and explain — 10 minutes

Cover the main path, a boundary and a failure:

- park, then checkout;
- 60 versus 61 minutes of hourly pricing;
- full capacity, duplicate plate or invalid ticket.

Explain the core invariant and the cost of a scan. Ordinary interview-scale configurations keep arithmetic and identifiers simple.

## 5. Discuss one follow-up — remaining time

For example, add a discount decorator or concurrent entrances. A mutex would span the whole park/checkout transition, not just individual map accesses. Calendar-based pricing needs a richer policy input than elapsed minutes alone.

The goal is a clear design that works within the agreed scope. More classes and more named patterns are not automatically a stronger answer.

[Pattern guide](design-patterns.md) · [Review checklist](review-checklist.md)
