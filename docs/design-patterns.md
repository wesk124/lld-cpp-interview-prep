# OOP and Design Patterns

The core examples are small enough to explain and implement in a 45–60-minute round. Each demonstrates a relevant pattern in working code. The catalog below is a reference, not a checklist to fit into one solution.

## OOP concepts

| Concept | Concrete example |
| --- | --- |
| Encapsulation | Game owns board/turn/status; Inventory owns stock and reservation transitions. |
| Abstraction | PricingPolicy, WinRule, AllocationPolicy, DispatchPolicy and RateLimiter describe varying behavior. |
| Polymorphism | A context calls its interface without choosing a concrete implementation. |
| Composition | A bank owns cars; a directory owns child nodes. |
| Borrowing | Pricing, dispatch and rate-limit policies are supplied by reference and outlive their clients. |
| Exclusive ownership | Directory owns children through unique_ptr<Node>, with a virtual base destructor. |
| Value semantics | Numeric IDs and copied booking/stock values avoid accidental shared ownership. |

SOLID discussions are concrete: coherent responsibilities, a change isolated by an interface, interchangeable implementations honoring a contract, and dependencies supplied by the caller.

## Patterns actually implemented

| Example | Pattern | Participants |
| --- | --- | --- |
| [Parking Lot](../solutions/01-parking-lot/design.md) | **Strategy** | `ParkingSpot`, `Ticket`, `ParkingLot`, `PricingPolicy` |
| [Connect Four](../solutions/02-connect-four/design.md) | **Strategy** | `Game`, `WinRule`, `ConnectKRule` |
| [Amazon Locker](../solutions/03-amazon-locker/design.md) | **Strategy** | `Slot`, `Locker`, `AllocationPolicy`, `SmallestFit` |
| [Elevator](../solutions/04-elevator/design.md) | **Strategy** | `Elevator`, `ElevatorBank`, `DispatchPolicy`, `NearestCar / LeastBusyCar` |
| [File System](../solutions/05-file-system/design.md) | **Composite** | `Node`, `File`, `Directory`, `FileSystem` |
| [Movie Ticket Booking](../solutions/06-movie-ticket-booking/design.md) | **Strategy** | `BookingService`, `Booking`, `SeatPricing`, `PerSeatPricing / BookingFeePricing` |
| [Logging Service](../solutions/07-logging-service/design.md) | **Observer + Adapter** | `Logger`, `Sink`, `MemorySink`, `StreamSink` |
| [Rate Limiter](../solutions/08-rate-limiter/design.md) | **Strategy** | `RateLimiter`, `TokenBucket`, `FixedWindow`, `RequestGate` |
| [Inventory Management](../solutions/09-inventory-management/design.md) | **Observer** | `Stock`, `Inventory`, `ReservationState`, `StockObserver` |

Strategy separates an interchangeable algorithm from the object coordinating its use. The rate-limiter strategies have different admission guarantees; a common interface does not make those guarantees identical.

Composite gives both files and directories the virtual size operation; a directory recursively delegates to child Nodes. Observer publishes log/stock events to registered interfaces. StreamSink is an Adapter for an existing ostream.

Enum-based lifecycle transitions are not the GoF State pattern. Token bucket and LOOK are algorithms; they become participants in a pattern through how objects collaborate. RAII, mutexes and dependency injection are techniques rather than GoF pattern names.

## Core versus follow-up

The core keeps numeric IDs, direct containers, small interfaces and a complete workflow. Payments, durable transactions, expiration, distributed coordination and asynchronous callbacks are follow-ups when the interviewer asks for them.

Most examples are single-threaded and borrow collaborators with explicitly documented lifetimes. Rate Limiter includes one mutex per policy because atomic admission is part of its core.

## GoF pattern catalog

This catalog lists all 23 patterns for reference. The possible applications below are study ideas; the main pattern map above identifies actual implementations.

### Creational: how objects are created

| Pattern | Intent | Possible application |
| --- | --- | --- |
| Factory Method | A creator defines a creation operation whose implementation subclasses can vary. | A configurable creator supplies a pricing policy; a standalone creation helper is a simple factory, not automatically Factory Method. |
| Abstract Factory | Create related objects through a family of creation interfaces. | Supply matching door, sensor, and display adapters for simulated or physical locker hardware. |
| Builder | Assemble a complex object through separate construction steps. | Assemble logging configuration with several sinks and wrappers. |
| Prototype | Create an object by copying a configured prototype. | Clone a game position for independent AI exploration, with explicit copy semantics. |
| Singleton | Provide one instance with a controlled global access point. | Discuss a process-wide registry and its lifetime, concurrency, and test-isolation tradeoffs versus passing dependencies explicitly. |

### Structural: how objects are composed

| Pattern | Intent | Possible application |
| --- | --- | --- |
| Adapter | Translate an existing interface into the interface a client expects. | `StreamSink` adapts stream output to `Sink::write`; a payment adapter could translate a vendor API. |
| Bridge | Let an abstraction and its implementation vary independently. | Separate locker-control features from alternative hardware communication backends. |
| Composite | Represent a part-whole tree through a common component interface. | File/directory nodes, or a group of sinks implementing `Sink`. |
| Decorator | Add behavior by wrapping an object behind the same interface. | A redacting sink or a pricing policy that adds a surcharge. |
| Facade | Expose a simpler entry point to collaborating components. | A follow-up path-based filesystem API could hide node navigation and validation. |
| Flyweight | Share immutable intrinsic data while keeping per-use state separate. | Share seat-category metadata while keeping each show's seat ownership separate. |
| Proxy | Control access to another object behind its interface. | Add authorization or remote-access handling around a booking service. |

### Behavioral: how objects collaborate

| Pattern | Intent | Possible application |
| --- | --- | --- |
| Chain of Responsibility | Pass a request through handlers until it is handled or the chain ends. | Route a log record through handlers with explicit acceptance/fallback rules. |
| Command | Represent an operation as an object that can be queued, recorded, or undone. | Connect Four drop commands with validated undo behavior. |
| Interpreter | Represent grammar rules and evaluate expressions in that language. | A filesystem search-expression language; splitting paths alone does not establish this pattern. |
| Iterator | Traverse a collection without exposing its representation. | Traverse a filesystem tree through a traversal interface. |
| Mediator | Centralize interactions among otherwise coupled collaborators. | Coordinate elevator cars, call panels, and displays through a controller. |
| Memento | Capture and restore an object's state while preserving encapsulation. | Restore a game's board, turn, move count, and terminal status. |
| Observer | Notify subscribed objects when an event or state change occurs. | Log-record delivery, inventory stock events, or locker pickup notifications. |
| State | Delegate behavior to objects representing the context's current state. | Separate held/booked/cancelled booking behavior as lifecycle rules expand. |
| Strategy | Encapsulate interchangeable algorithms behind a common contract. | Pricing, dispatch, code generation, or alternative admission algorithms. |
| Template Method | Define an algorithm skeleton with overridable steps in a base class. | A delivery workflow with variable authentication and notification steps. |
| Visitor | Add operations to an object hierarchy using visitor dispatch. | Filesystem size reports and exports, accepting the cost of updating visitors when node kinds change. |

## Explain the pattern in an interview

> Hourly and flat fees vary independently of spot allocation. ParkingLot borrows a PricingPolicy and calls fee(elapsed_minutes). This is Strategy. The caller keeps the policy alive, and checkout calculates the fee before releasing occupancy.

Then name one tradeoff. The interface adds indirection but isolates the expected change. A calendar-based rule would need more input than elapsed minutes; describe that interface change rather than assuming every future rule fits.

[Pattern quiz](../quizzes/design-patterns-questions.md) · [Interview playbook](interview-playbook.md)
