# Inventory Management: Reference Design

[Question](../../questions/09-inventory-management/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Inventory owns SKU Stock values and an order Reservation ledger. A reservation retains its original quantities and terminal state for idempotent retries.

One mutex covers all SKUs in a reservation. Validate the entire order and all available quantities before creating the ledger record or changing reserved counts.

The invariant is 0 <= reserved <= on_hand. Commit subtracts from both counts; release subtracts only reserved, so availability and physical stock remain distinct.

Matching held/committed reserve retries acknowledge the original operation. Different payloads for the same order ID are rejected; a released order ID cannot silently start a new reservation.

For K order lines and N SKUs, reserve/commit/release are O(K log N + log O), with O orders in the ledger. Space grows with catalog size and retained order lines.

This is not an event-sourced or durable inventory system. Production adds durable transaction/idempotency boundaries, retention, warehouse ownership, audit events, and expiration if required.


## Scope

One in-memory catalog, no warehouses, prices, shipment, payment, persistence, or reservation timeout. Unknown order commit/release returns false. Integer overflow throws. Completed order records remain available for retry deduplication.

## Extend it yourself

- Add multiple warehouses and transfer workflows.
- Add reservation expiration and low-stock notifications.
- Map operations to database transactions and durable idempotency records.
