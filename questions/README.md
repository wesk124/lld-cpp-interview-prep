# Interview Questions (C++17)

Every starter has an explicit public contract and marked TODOs. Implement your own state and behavior before inspecting `solutions/`.

| Question | Main concepts |
| --- | --- |
| [Parking Lot](01-parking-lot/README.md) | Ownership, Strategy, compatible allocation, atomic checkout |
| [Connect Four](02-connect-four/README.md) | Board modeling, gravity, directional win detection, terminal states |
| [Amazon Locker](03-amazon-locker/README.md) | Size-based allocation, injected codes, expiration, physical occupancy |
| [Elevator](04-elevator/README.md) | Door interlocks, LOOK scheduling, bank dispatch Strategy |
| [File System](05-file-system/README.md) | Composite hierarchy, unique ownership, path validation, traversal |
| [Movie Ticket Booking](06-movie-ticket-booking/README.md) | Atomic seat holds, state machine, expiry, idempotency |
| [Logging Service](07-logging-service/README.md) | Sink interfaces, fanout, failure isolation, lock/lifetime boundaries |
| [Rate Limiter](08-rate-limiter/README.md) | Token-bucket invariants, weighted requests, monotonic time, contention |
| [Inventory Management](09-inventory-management/README.md) | Stock invariants, multi-SKU transactions, idempotent lifecycle |

Run any attempt with `bash scripts/test.sh <directory-name> practice` from the repository root, or use the CMake instructions in its prompt. The incomplete starter is expected to fail; it never includes reference code.
