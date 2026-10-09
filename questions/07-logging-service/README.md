# Logging Service

Design a synchronous logging service with severity filtering and multiple output sinks.

## Interview contract

1. Support debug/info/warning/error levels, with a configurable minimum.
2. Accept injected record timestamps and fan out accepted records to all registered sinks.
3. Provide in-memory and ostream sinks.
4. Count per-sink delivery failures without preventing delivery to remaining sinks.
5. Support concurrent producers and configuration updates; never hold the logger configuration lock while invoking arbitrary sink code.

## Scope and assumptions

Synchronous in-process logging only. No background queue, rotation, network delivery, global record ordering, or guaranteed persistence. Shared ownership is deliberate because callers and multiple loggers may retain a sink. StreamSink borrows its ostream; the caller must keep it alive and avoid unsynchronized direct access. Custom sinks must be thread-safe.

## Interviewee TODOs

- [ ] Define immutable-by-convention Record and Delivery value types.
- [ ] Define a sink interface with a virtual destructor.
- [ ] Implement level filtering and a shared-lifetime sink snapshot.
- [ ] Synchronize each provided sink independently.
- [ ] Test threshold changes, broken sinks/streams, fanout, and concurrent writes.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## OOP and pattern discussion

- [ ] Explain the Sink abstraction, shared sink ownership, and borrowed stream lifetime; identify StreamSink's Adapter role.
- [ ] Compare Observer-style fanout with Chain of Responsibility, including whether one sink handles a record or all receive it.
- [ ] Explore a redacting Decorator or Composite sink group and define synchronization, wrapper ordering, and delivery semantics.

[Pattern guide](../../docs/design-patterns.md). These discussion extensions are separate from the base test contract.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=07-logging-service
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 07-logging-service practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add a bounded asynchronous queue with an explicit overflow policy.
2. Add formatting/redaction and rotating-file sinks.
3. Define flush/shutdown ordering and durability guarantees.

After your attempt: [design explanation](../../solutions/07-logging-service/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
