# Interview-Sized Reference Solutions

Each solution is one small header with its tests and a short explanation. They model a core workflow and an explicit pattern rather than a production service.

| Example | Design | Code | Pattern |
| --- | --- | --- | --- |
| Parking Lot | [Explanation](01-parking-lot/design.md) | [Implementation](01-parking-lot/solution.hpp) | Strategy |
| Connect Four | [Explanation](02-connect-four/design.md) | [Implementation](02-connect-four/solution.hpp) | Strategy |
| Amazon Locker | [Explanation](03-amazon-locker/design.md) | [Implementation](03-amazon-locker/solution.hpp) | Strategy |
| Elevator | [Explanation](04-elevator/design.md) | [Implementation](04-elevator/solution.hpp) | Strategy |
| File System | [Explanation](05-file-system/design.md) | [Implementation](05-file-system/solution.hpp) | Composite |
| Movie Ticket Booking | [Explanation](06-movie-ticket-booking/design.md) | [Implementation](06-movie-ticket-booking/solution.hpp) | Strategy |
| Logging Service | [Explanation](07-logging-service/design.md) | [Implementation](07-logging-service/solution.hpp) | Observer + Adapter |
| Rate Limiter | [Explanation](08-rate-limiter/design.md) | [Implementation](08-rate-limiter/solution.hpp) | Strategy |
| Inventory Management | [Explanation](09-inventory-management/design.md) | [Implementation](09-inventory-management/solution.hpp) | Observer |

Run all reference tests with `bash scripts/test.sh` or the root CMake project. Explain the roles and ownership, then compare one alternative design.

[Pattern map and catalog](../docs/design-patterns.md)
