# Inventory Management: Core Test Checklist

- [ ] No partial reservation when one SKU lacks stock.
- [ ] Commit/release accounting; retry idempotency; changed-payload rejection.
- [ ] Low-stock Observer delivery and unsubscribe; basic catalog input checks.
- [ ] Name the pattern participants and verify polymorphic behavior where relevant.

The checklist matches the small base contract. Follow-up features have separate design discussions rather than extra base-test requirements.
