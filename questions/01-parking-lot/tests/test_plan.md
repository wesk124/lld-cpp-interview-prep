# Test Plan TODO

- [ ] A motorcycle selects a motorcycle spot before compact or large.
- [ ] A car selects a compact spot before large.
- [ ] A truck selects only a large spot.
- [ ] Parking fails when no compatible spot exists.
- [ ] A duplicate active license plate is rejected.
- [ ] Checkout rounds partial hours according to the chosen policy.
- [ ] Checkout frees the spot and makes a ticket single-use.
- [ ] Exit before entry is rejected without mutating state.
- [ ] Concurrent operations preserve capacity and active-ticket invariants.
