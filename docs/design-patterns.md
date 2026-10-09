# OOP and Design Patterns

These exercises develop object-oriented design through responsibilities, interfaces, ownership, and changing requirements. A useful pattern explanation names the participating objects and the change their collaboration supports.

## OOP concepts in the examples

| Concept | What to explain | Example |
| --- | --- | --- |
| Encapsulation | Which object protects the state and its invariants? | `Game::drop` coordinates the board, turn, and terminal status; `Inventory` coordinates stock and reservations. |
| Abstraction | What behavior does a caller need without knowing the implementation? | `PricingPolicy::calculate`, `DispatchPolicy::choose`, and `Sink::write`. |
| Inheritance and substitution | Can each derived object honor the base interface and its behavioral contract? | `HourlyPricingPolicy` implements `PricingPolicy`; `MemorySink` and `StreamSink` implement `Sink`. |
| Polymorphism | Which behavior changes when a different implementation is supplied? | A `ParkingLot` invokes its pricing policy through the base interface. |
| Composition | Which objects collaborate, and who owns them? | `ElevatorBank` owns cars and a dispatch policy; `Directory` owns child nodes. |
| Dependency injection | Where are collaborators supplied, and how can tests replace them? | Constructor-supplied pricing/dispatch policies and the locker's code-generation callable. |
| Value semantics | Which results can be copied without borrowing mutable internal state? | `Move`, `Ticket`, `Booking`, and `Stock` snapshots. |

For SOLID discussions, connect each principle to a concrete decision: coherent responsibilities (SRP), adding a pricing policy through the interface (OCP), honoring that interface's contract (LSP), small client-facing interfaces (ISP), and depending on supplied policy abstractions (DIP). These are discussion tools rather than a class-count target.

## Pattern map for the nine examples

The **present** column describes the reference code. The **follow-up** column proposes design exercises; those patterns are not implemented there. Some examples emphasize encapsulation and explicit state transitions without a GoF pattern.

| Example | Present in the reference | Follow-up patterns and the change they support |
| --- | --- | --- |
| [Parking Lot](../solutions/01-parking-lot/design.md) | **Strategy**: `ParkingLot` owns a `PricingPolicy`; `HourlyPricingPolicy` supplies the calculation. | **Decorator** to compose discounts or surcharges around a pricing policy; **Observer** for capacity updates. |
| [Connect Four](../solutions/02-connect-four/design.md) | Encapsulated rules engine and an enum-based lifecycle. | **Strategy** for human/AI move selection; **Command** for move execution and undo; **State** if lifecycle behavior grows. |
| [Amazon Locker](../solutions/03-amazon-locker/design.md) | **Strategy via a callable**: `Locker` receives a replaceable code generator. Allocation remains a fixed algorithm. | **Strategy** for allocation; **Adapter** for door hardware; **Observer** for pickup/expiry notifications; **State** for a richer package lifecycle. |
| [Elevator](../solutions/04-elevator/design.md) | **Strategy**: `ElevatorBank` delegates car selection to `DispatchPolicy` / `NearestCarPolicy`. Door and direction transitions use enums. | **State** for normal/emergency/maintenance behavior; **Observer** for displays and arrival notifications. |
| [File System](../solutions/05-file-system/design.md) | **Composite-style hierarchy**: `Directory` contains `Node` children, including `File` and other directories. **Facade**: `FileSystem` exposes path-based operations. | **Visitor** for new tree-wide operations; **Command** for undoable edits. |
| [Movie Ticket Booking](../solutions/06-movie-ticket-booking/design.md) | A service coordinating seat ownership and an explicit `HoldStatus` lifecycle. | **State** for richer hold behavior; **Strategy** for pricing; **Adapter** for an external payment API. |
| [Logging Service](../solutions/07-logging-service/design.md) | **Adapter**: `StreamSink` exposes an `ostream` as a `Sink`. **Observer-style fanout**: `Logger` pushes records to registered sinks. | **Decorator** for redaction/formatting wrappers; **Composite** for sink groups that also implement `Sink`. |
| [Rate Limiter](../solutions/08-rate-limiter/design.md) | Encapsulated per-client state and one token-bucket algorithm. | **Strategy** for interchangeable admission algorithms; **Decorator** for adding admission checks around a service interface. |
| [Inventory Management](../solutions/09-inventory-management/design.md) | Encapsulated `Stock` values and an enum-based reservation lifecycle. | **Strategy** for warehouse selection; **Observer** for stock events; **State** for an expanded reservation lifecycle. |

A state machine implemented with enums and conditionals is different from the GoF **State** pattern, where a context delegates behavior to state objects. Similarly, a token bucket and LOOK scheduling are algorithms; mutexes, RAII, and dependency injection are techniques or idioms rather than GoF pattern names.

The filesystem implements the Composite ownership structure with a small `Node::is_directory` interface. It does not currently put every file/directory operation behind a uniform recursive `Node` operation. The logger's fanout synchronously attempts every sink; it has no subscription-removal API and does not implement a Chain of Responsibility that stops at a handling sink.

## GoF pattern catalog

This catalog lists all 23 patterns for reference. The possible applications below are study ideas, not claims about existing implementations or requirements to use them.

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
| Facade | Expose a simpler entry point to collaborating components. | `FileSystem` hides node navigation, path validation, and tree operations behind path-based methods. |
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

## Explain a pattern in an interview

Start with the anticipated change, then name the interface, concrete collaborators, and owner. For example:

> Weekend pricing changes fee calculation. `ParkingLot` owns a `PricingPolicy` and calls `calculate`; another implementation can supply weekend rules. This is Strategy. The policy runs inside the checkout lock, so its contract also includes bounded, non-reentrant execution.

Then discuss the tradeoff: more interfaces and indirection versus a change that can be isolated and tested. An enum-based lifecycle can remain a clear choice when a small number of transitions fits comfortably in one object.

[Pattern quiz](../quizzes/design-patterns-questions.md) · [Interview playbook](interview-playbook.md) · [Questions](../questions/README.md)
