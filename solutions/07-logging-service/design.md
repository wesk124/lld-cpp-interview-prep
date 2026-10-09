# Logging Service: Reference Design

[Question](../../questions/07-logging-service/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Logger coordinates filtering and fanout through the Sink interface. MemorySink stores record values; StreamSink formats to a borrowed ostream.

Logger and callers intentionally share sink lifetime through shared_ptr. Stream ownership is different: StreamSink does not own or extend the lifetime of the underlying stream.

Under the logger mutex, log checks severity and copies the sink list. It releases that lock before invoking sinks, avoiding a configuration-lock dependency on arbitrary output code.

Provided sinks protect their own state. A sink exception increments failed and does not skip the remaining sinks; one call returns delivery counts rather than silently claiming success.

With N sinks and M-byte messages, fanout is O(N + output work); copying and storing memory records adds O(M). MemorySink grows with retained records and returns copied snapshots.

Different threads may interleave deliveries in different sink orders. Global ordering needs one queue/consumer; a production asynchronous logger must define backpressure, flushing, redaction, and retention.


## Scope

Synchronous in-process logging only. No background queue, rotation, network delivery, global record ordering, or guaranteed persistence. Shared ownership is deliberate because callers and multiple loggers may retain a sink. StreamSink borrows its ostream; the caller must keep it alive and avoid unsynchronized direct access. Custom sinks must be thread-safe.

## Extend it yourself

- Add a bounded asynchronous queue with an explicit overflow policy.
- Add formatting/redaction and rotating-file sinks.
- Define flush/shutdown ordering and durability guarantees.
