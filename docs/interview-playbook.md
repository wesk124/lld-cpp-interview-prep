# LLD Interview Playbook

## 1. Clarify before modeling

Ask about actors, primary workflows, capacity, persistence, concurrency, failure behavior, and what is explicitly out of scope. Repeat the agreed scope in one or two sentences.

For a parking lot, useful questions include:

- Which vehicle and spot types exist?
- Can a smaller vehicle use a larger spot?
- How is pricing calculated?
- Are reservations, multiple entrances, and persistence required?
- Can park and exit requests happen concurrently?

## 2. Identify behavior, not just nouns

Start from use cases. Assign each behavior to the object with the information required to perform it. Behaviors and invariants help decide which nouns deserve their own classes.

State important invariants:

- One spot contains at most one vehicle.
- One active ticket maps to exactly one occupied spot.
- Checkout removes the active ticket and frees its spot atomically.

## 3. Make ownership explicit

For every relationship, say whether it is ownership, observation, or identification. Values and `std::unique_ptr` express exclusive ownership; `std::shared_ptr` expresses shared lifetime ownership. Stable IDs often avoid dangling cross-object pointers.

## 4. Isolate expected change

Introduce an interface when a requirement is expected to vary. Pricing is a good Strategy candidate because hourly, weekend, event, and membership policies can change independently of parking allocation.

An abstraction is easier to justify when you can finish this sentence:

> This abstraction isolates changes to ____ from ____.

Use the [OOP and design-pattern guide](design-patterns.md) to connect a pattern to its participants: the context, interface, implementations, and owner. Explain which pattern is already present and which would support a follow-up requirement. For example, `DispatchPolicy` is Strategy; enum-based elevator transitions could become State objects if their behavior grows. Describe the change and tradeoff before adding the extra objects.

## 5. Implement a vertical slice

Write enough code to demonstrate the main workflow end-to-end. Favor compilable interfaces and one correct path over a large collection of empty classes.

## 6. Close with operational concerns

Discuss:

- invalid inputs and state transitions
- shared mutable state and lock boundaries
- persistence and recovery if relevant
- deterministic testing
- the first change you would make for production scale

## C++ signals interviewers notice

`enum class`, `const`, `explicit`, `override`, RAII, value semantics, and virtual destructors can help express a design. Explain the lifetime and ownership behind values, smart pointers, references, and stable IDs, and discuss risks such as slicing or unintended shared state.
