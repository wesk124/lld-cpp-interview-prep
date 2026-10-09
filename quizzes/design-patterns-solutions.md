# OOP and Pattern Explanations

1. **Parking Lot:** ParkingLot is the context, PricingPolicy the interface, and HourlyPricing/FlatPricing the implementations. The lot borrows the policy; the caller owns its lifetime.

2. **Connect Four:** Game delegates winning behavior to WinRule / ConnectKRule. Status is an enum-based lifecycle, not delegated State objects.

3. **Amazon Locker:** Selection can vary without changing deposit/pickup and assignment bookkeeping. SmallestFit returns a compatible slot index.

4. **Elevator:** DispatchPolicy selects a car for the bank. LOOK orders a car's destinations. The object collaboration is Strategy; LOOK itself is an algorithm.

5. **File System:** Node is the component, File the leaf and Directory the composite. Virtual size returns file bytes or recursively totals child Nodes.

6. **Movie Ticket Booking:** SeatPricing computes fees through PerSeatPricing/BookingFeePricing. BookingService validates and owns seat/booking state. It borrows the pricing collaborator.

7. **Logging Service:** Logger publishes to registered Sink observers. MemorySink receives/stores values; StreamSink adapts ostream to Sink. Unsubscribe does not destroy a borrowed sink.

8. **Rate Limiter:** It borrows RateLimiter and invokes allow through that interface. TokenBucket and FixedWindow can be supplied without editing gate code, though their quota semantics differ.

9. **Inventory Management:** Inventory publishes low-stock facts through StockObserver instead of naming a notification implementation. It still owns reservation accounting, whole-order validation and idempotent transitions.

[Questions](design-patterns-questions.md) · [Pattern guide](../docs/design-patterns.md)
