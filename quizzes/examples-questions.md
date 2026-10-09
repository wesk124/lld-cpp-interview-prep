# Interview-Core Scenario Quiz

Use the current small core contracts, not the earlier extended APIs.

## Parking Lot

1. A ticket has an integer ID. What would a string prefix add to the core design?
2. An exit precedes entry. Should the spot be freed?
3. Where does a flat fee belong?

## Connect Four

1. A move targets a full column. Whose turn is next?
2. Why check lines through the newest piece?
3. Which object decides whether a board position wins?

## Amazon Locker

1. Why choose the smallest compatible slot?
2. Can the same pickup code retrieve a package twice?
3. Are sequential integer pickup codes secure authentication?

## Elevator

1. A car opened its doors at a stop. Can the next step also move?
2. Does LOOK reverse immediately for a newly requested stop behind the car?
3. Which two selection policies use the same interface?

## File System

1. Why can a caller ask either a file or directory for size through Node?
2. Who owns a directory's children?
3. Can a borrowed child pointer be used after removing its subtree?

## Movie Ticket Booking

1. One of two requested seats is already booked. May the other be taken?
2. Why calculate price before marking seats?
3. Does the base implementation prevent concurrent double booking?

## Logging Service

1. Does a record stop after the first sink accepts it?
2. What does StreamSink adapt?
3. Does removing a sink destroy it?

## Rate Limiter

1. A bucket is empty and refills at 2 tokens/second. What is available after 1.5 seconds?
2. Do TokenBucket and FixedWindow provide identical admission guarantees?
3. Which operations belong under the same mutex?

## Inventory Management

1. A second SKU is unavailable. May the first SKU remain reserved?
2. What changes when a reservation of four is committed from on_hand=10, reserved=4?
3. Where do low-stock delivery rules live relative to accounting?

[Answer key](examples-solutions.md)
