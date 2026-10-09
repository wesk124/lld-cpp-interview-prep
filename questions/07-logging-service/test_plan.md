# Logging Service: Core Test Checklist

- [ ] Severity filtering and changes to the threshold.
- [ ] Fanout reaches every Observer; unsubscribed sinks stop receiving records.
- [ ] StreamSink adapts formatted records to an ostream.
- [ ] Name the pattern participants and verify polymorphic behavior where relevant.

The checklist matches the small base contract. Follow-up features have separate design discussions rather than extra base-test requirements.
