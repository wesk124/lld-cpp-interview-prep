# OOP and Design-Pattern Quiz

For each answer, name the collaborating objects, the requirement that motivates the pattern, and a tradeoff. Follow-up features mentioned here are design exercises rather than claims about the reference implementations.

1. **Parking Lot:** Weekend fees replace hourly fees without changing allocation or checkout coordination. Which pattern fits, and which object owns the varying behavior? How would a surcharge wrapper differ?

2. **Connect Four:** `Game` has a `Status` enum and checks it in `drop`. Does this implement the GoF State pattern? Which patterns could support an AI move selector and undoable moves?

3. **Amazon Locker:** What pattern role does the supplied code-generation callable play? A vendor door API uses a different interface from your domain model: which pattern could connect them?

4. **Elevator:** Which collaboration implements Strategy? Is LOOK itself a design pattern? What would change if normal, emergency, and maintenance modes delegated behavior to separate objects?

5. **File System:** Identify the component, leaf, and composite roles. Why is the current hierarchy described as Composite-style? How could Visitor add a size-report operation, and what happens when a new node type is introduced?

6. **Movie Ticket Booking:** Seat pricing varies, and an external payment provider has a vendor-specific API. Which two patterns address these changes? Would replacing `HoldStatus` with State objects by itself prevent double booking?

7. **Logging Service:** The logger attempts every registered sink. Is that a Chain of Responsibility? Which pattern does `StreamSink` demonstrate? How could you add redaction while keeping the `Sink` interface?

8. **Rate Limiter:** Is a token bucket the Strategy pattern? Describe the interface and collaborators that would let token-bucket and sliding-window implementations be selected independently of a calling service.

9. **Inventory Management:** Which pattern could deliver low-stock events to notification clients? Where should callbacks run relative to the inventory lock, and which invariants remain the inventory object's responsibility?

[Answer key](design-patterns-solutions.md) · [Study guide](../docs/design-patterns.md) · [Quiz index](README.md)
