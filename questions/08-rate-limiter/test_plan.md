# Rate Limiter: Core Test Checklist

- [ ] Continuous/partial refill, burst capacity and weighted cost.
- [ ] Independent clients, invalid cost and backward-time rejection.
- [ ] Polymorphic policy selection; fixed-window boundary; concurrent admission cannot exceed burst.
- [ ] Name the pattern participants and verify polymorphic behavior where relevant.

The checklist matches the small base contract. Follow-up features have separate design discussions rather than extra base-test requirements.
