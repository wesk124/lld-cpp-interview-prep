# Logging Service

A 45–60-minute OOP exercise. Main pattern: **Observer + Adapter**.

## Core interview contract

1. Filter records below the configured minimum severity.
2. Publish each accepted record to every registered sink.
3. Support registration/removal without changing sink implementations.
4. Provide in-memory and stream sinks through the same Sink interface.

## Scope and assumptions

Single-threaded synchronous delivery with valid severity values. Sinks and streams outlive registration. Base callbacks succeed and do not mutate the logger/subscriptions; async queues, failures and concurrency are follow-ups.

## Interviewee TODOs

- [ ] Define Sink with a virtual destructor and implement both sinks.
- [ ] Keep Logger independent of concrete sink classes: Observer.
- [ ] Adapt ostream through StreamSink and demonstrate unsubscribe.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 07-logging-service practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=07-logging-service` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add a redacting Decorator or a Composite sink group.
- Add async delivery, synchronization, backpressure and failure isolation after defining their contracts.

[After your attempt: design](../../solutions/07-logging-service/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
