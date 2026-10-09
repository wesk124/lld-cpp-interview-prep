# OOP Interview Foundations: Explanations

1. **B.** A reference borrows a collaborator; it does not transfer ownership. Construct the pricing policy first and keep it alive while the lot uses it.
2. **A.** The spot ID identifies the spot without depending on a vector element's memory address. Numeric IDs are sufficient for this model.
3. **B.** A whole state transition needs one invariant boundary; separate locking around individual map/spot accesses can leave an inconsistent workflow.
4. **B.** Pricing, allocation, dispatch and winning rules are examples of behavior that can vary independently of coordination.
5. **B.** Directory owns children through the base type, so derived destructors must run.
6. One occupied spot per active ticket; one active ticket per plate; checkout frees exactly the ticket's spot and makes that ticket unusable.
7. The lot has a clear ownership/lifetime boundary, and each spot's small state has ordinary value semantics.
8. The spot occupancy, ticket map and ticket-number assignment for park; occupancy and ticket removal for checkout. Pricing/I/O contracts need discussion if they execute under a lock.
9. Supply integer minute values directly: entry 0, checkout 60 or 61.
10. Supply a FlatPricing object through PricingPolicy. ParkingLot still calls fee and keeps the same allocation/checkout workflow.

[Questions](questions.md)
