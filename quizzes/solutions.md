# C++ LLD Foundations Quiz Solutions

1. **B.** `std::unique_ptr` expresses exclusive ownership while allowing runtime polymorphism.
2. **B.** A stable ID remains valid independently of container storage and object movement.
3. **C.** Locking exists to preserve invariants over shared mutable state; function-level locking is only an implementation technique.
4. **B.** Strategy is useful when it isolates an expected family of policy changes.
5. **B.** Deleting a derived policy through a base pointer requires a virtual base destructor.
6. Examples: a spot contains at most one vehicle; a plate has at most one active ticket; an active ticket identifies exactly one occupied spot.
7. Spots have clear ownership, uniform lifetime, and natural value behavior. Storing them directly reduces allocation and avoids unnecessary pointer ownership.
8. The occupied spot, active-ticket map, active-license set, and any completed-session or payment state that must change consistently.
9. Pass entry and exit timestamps into the operation or inject a clock interface. Tests then use fixed time points.
10. Add or select a new `PricingPolicy` implementation. Allocation, spot occupancy, and active-ticket coordination should not change.
