# Logging Service: Interview Design

[Question](../../questions/07-logging-service/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Observer + Adapter**.

| Object | Responsibility |
| --- | --- |
| `Logger` | Severity filtering and publishing records to subscribers. |
| `Sink` | Observer interface for record delivery. |
| `MemorySink` | Observer retaining record values. |
| `StreamSink` | Adapter translating Sink.write into ostream output. |

log filters once, constructs a record and invokes write on every sink. StreamSink translates that call into formatted stream output.

## Ownership and scope

Logger borrows sinks through non-owning pointers supplied by reference. StreamSink borrows its ostream. Removal unregisters a sink; it does not destroy it.

Single-threaded synchronous delivery with valid severity values. Sinks and streams outlive registration. Base callbacks succeed and do not mutate the logger/subscriptions; async queues, failures and concurrency are follow-ups.

## Small usage example

Within the example's namespace:

```cpp
MemorySink memory;
Logger logger;
logger.add_sink(memory);
logger.log(Level::info, "started");
logger.remove_sink(memory);
// memory.records().size() == 1
```

Delivery is O(sinks + output work). MemorySink stores retained records without a retention policy.

## Follow-up discussion

- Add a redacting Decorator or a Composite sink group.
- Add async delivery, synchronization, backpressure and failure isolation after defining their contracts.

[Pattern map and catalog](../../docs/design-patterns.md)
