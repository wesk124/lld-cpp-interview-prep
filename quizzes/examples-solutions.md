# Interview-Core Scenario Explanations

## Parking Lot

1. Only display formatting; numeric identity is enough.
2. No. checkout returns -1 and leaves the active session occupied.
3. In a PricingPolicy implementation, not a pricing switch inside ParkingLot.

## Connect Four

1. The same player's; the invalid move returns false without changing turn/state.
2. Only that piece can create a new win after a previously non-winning board.
3. The WinRule Strategy; Game coordinates the drop and lifecycle.

## Amazon Locker

1. To avoid occupying a large slot unnecessarily and preserve it for a large package.
2. No. Pickup erases the assignment and releases the slot.
3. No. They demonstrate lookup only; secure authentication is a follow-up.

## Elevator

1. No. Closing consumes that step and the floor remains unchanged.
2. No. It finishes pending destinations ahead, then reverses.
3. NearestCar and LeastBusyCar implement DispatchPolicy.

## File System

1. Composite exposes the same operation; a directory recursively sums its children.
2. The directory, through unique_ptr<Node>.
3. No. Removal destroys the owned subtree and invalidates that pointer.

## Movie Ticket Booking

1. No. The complete request is validated before any seats change.
2. The pricing step can complete without leaving partially updated seat ownership if it fails.
3. The base is single-threaded. A concurrent extension needs a lock spanning checks and updates.

## Logging Service

1. No. Logger notifies all registered sinks; this is Observer, not a handling chain.
2. An existing ostream output interface into Sink.write.
3. No. Logger borrows sinks; removing a subscription does not transfer/destroy ownership.

## Rate Limiter

1. Three tokens, subject to the capacity cap.
2. No. They share an interface but have different continuous-refill/window-boundary semantics.
3. Client-state lookup, refill/window advancement, capacity checking and consumption.

## Inventory Management

1. No. Whole-order validation precedes all reserved-count updates.
2. on_hand becomes 6 and reserved becomes 0; available stays 6.
3. Inventory updates the full order first, then notifies the StockObserver interface. Base callbacks do not mutate inventory.

[Questions](examples-questions.md)
