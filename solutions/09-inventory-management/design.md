# Inventory Management: Interview Design

[Question](../../questions/09-inventory-management/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Observer**.

| Object | Responsibility |
| --- | --- |
| `Stock` | On-hand/reserved counts and available quantity. |
| `Inventory` | Catalog, numeric order IDs and whole-order reservations. |
| `ReservationState` | Simple held/committed/released lifecycle. |
| `StockObserver` | Subscriber receiving low-stock events. |

reserve checks every SKU, records the order, updates all reservations, then notifies low-stock observers. commit consumes once; release returns held quantities to availability.

## Ownership and scope

Inventory owns stock and reservation values and borrows observers. stock(sku) returns a value copy. Subscriptions do not own or destroy clients.

Single-threaded in-memory catalog with numeric order IDs and text SKU names. Notifications occur after the full mutation, use successful non-mutating callbacks, and may repeat while stock remains low. Subscribers outlive registration. Warehouses, expiration and durable storage are follow-ups.

## Small usage example

Within the example's namespace:

```cpp
Inventory inventory(3);
inventory.add_sku("SKU-A", 10);
inventory.reserve(101, {{"SKU-A", 4}});
inventory.commit(101);
// inventory.stock("SKU-A").on_hand == 6
```

K-line orders use O(K log SKUs + log orders), plus Observer delivery. Completed order records remain for retry detection.

## Follow-up discussion

- Add warehouse-selection Strategy or an expanded reservation State design.
- Add concurrency, durable idempotency, callback failure rules and event ordering to the model.

[Pattern map and catalog](../../docs/design-patterns.md)
