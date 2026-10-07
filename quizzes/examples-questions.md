# Nine-Example Scenario Quiz

Answer before opening the separate solution file. These questions test invariants and tradeoffs, not pattern-name memorization.

## Parking Lot

1. A custom pricing policy throws during checkout. Should the spot become available?
2. A car cannot use the remaining motorcycle spot. Is the lot necessarily full?
3. Why use stable spot IDs in tickets rather than vector element pointers?

## Connect Four

1. After an illegal move into a full column, whose turn is it?
2. Must you rescan the whole board to detect a win after a legal move?
3. Why maintain a move count as well as the board?

## Amazon Locker

1. A pickup code expires at 12:00. Can another package use that slot at 12:01 without courier collection?
2. The code generator returns an active code again. Should deposit silently overwrite the old assignment?
3. Would the deterministic counter used in tests be suitable for production pickup authentication?

## Elevator

1. The car reaches floor 3, opens its doors, and has another stop at 5. May the next tick move to 4?
2. A car is moving up with a pending stop above it; a new request arrives below. Does LOOK reverse immediately?
3. Does a nearest-car dispatcher automatically solve passenger-direction matching and starvation?

## File System

1. Who should own a directory's child nodes?
2. Can writing /a/b/file succeed when /a exists but /a/b does not?
3. Why should the public read/list APIs return values rather than mutable Node pointers?

## Movie Ticket Booking

1. Two callers simultaneously request the last seat. What must be inside the same critical section?
2. A hold expires at time T. Is confirmation at T valid?
3. Why preserve terminal hold states instead of releasing seats whenever an old ID is cancelled?

## Logging Service

1. Should Logger retain its configuration mutex while calling arbitrary Sink::write code?
2. One sink throws. Must the remaining sinks lose the record?
3. Do two synchronized sinks imply all threads' records appear in identical order in both sinks?

## Rate Limiter

1. A bucket has capacity 5 and refills at 2 tokens/second. It is empty; 1.5 seconds pass. How many tokens are available?
2. Does a token bucket guarantee at most N requests in every fixed one-second window?
3. Why must refill and token subtraction be atomic together?

## Inventory Management

1. An order reserves two SKUs; the second SKU has insufficient quantity. May the first SKU remain reserved?
2. An identical reserve or commit request is retried with the same order ID. Should stock change twice?
3. An item has on_hand=10 and reserved=4. What is available, and what changes when all four are committed?

[Answer key](examples-solutions.md)
