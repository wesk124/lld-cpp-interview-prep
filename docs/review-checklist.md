# Design Review Checklist

Score each category from 0 to 2. A consistent score of 14/18 is a useful readiness target.

| Category | Question |
| --- | --- |
| Requirements | Did I clarify ambiguity and define scope? |
| Responsibilities | Does each class have one coherent purpose? |
| Interfaces | Can expected policies vary without editing core domain classes? |
| Ownership | Is every object's lifetime unambiguous? |
| Extensibility | Can I handle the interviewer's follow-up cleanly? |
| Correctness | Are invariants and invalid state transitions enforced? |
| Concurrency | Did I identify shared state and lock boundaries? |
| Testing | Did I cover normal, boundary, and failure paths? |
| Communication | Did I explain tradeoffs instead of silently coding? |

## Warning signs

- Every noun becomes a class.
- Inheritance is used only for code reuse.
- `std::shared_ptr` is the default ownership model.
- A Singleton hides dependency or lifetime management.
- A pattern is named without identifying the change it isolates.
- Thread safety is claimed without naming the protected state.
- The design cannot be tested without wall-clock time or global state.
