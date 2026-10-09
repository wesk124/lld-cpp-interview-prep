# C++ OOP Interview Practice

Nine small low-level-design examples for a **45–60-minute interview**. Each core shows a working object model, clear responsibilities and a relevant design pattern. The current examples and build scripts use C++11.

The reference solutions are single headers of roughly 60–120 lines, including declarations and includes. Numeric identifiers use int or long long; strings represent actual text such as plates, filenames and SKU names.

## Questions and solutions

| TODO question | Reference solution | Main pattern |
| --- | --- | --- |
| [Parking Lot](questions/01-parking-lot/README.md) | [Code](solutions/01-parking-lot/solution.hpp) / [design](solutions/01-parking-lot/design.md) | Strategy |
| [Connect Four](questions/02-connect-four/README.md) | [Code](solutions/02-connect-four/solution.hpp) / [design](solutions/02-connect-four/design.md) | Strategy |
| [Amazon Locker](questions/03-amazon-locker/README.md) | [Code](solutions/03-amazon-locker/solution.hpp) / [design](solutions/03-amazon-locker/design.md) | Strategy |
| [Elevator](questions/04-elevator/README.md) | [Code](solutions/04-elevator/solution.hpp) / [design](solutions/04-elevator/design.md) | Strategy |
| [File System](questions/05-file-system/README.md) | [Code](solutions/05-file-system/solution.hpp) / [design](solutions/05-file-system/design.md) | Composite |
| [Movie Ticket Booking](questions/06-movie-ticket-booking/README.md) | [Code](solutions/06-movie-ticket-booking/solution.hpp) / [design](solutions/06-movie-ticket-booking/design.md) | Strategy |
| [Logging Service](questions/07-logging-service/README.md) | [Code](solutions/07-logging-service/solution.hpp) / [design](solutions/07-logging-service/design.md) | Observer + Adapter |
| [Rate Limiter](questions/08-rate-limiter/README.md) | [Code](solutions/08-rate-limiter/solution.hpp) / [design](solutions/08-rate-limiter/design.md) | Strategy |
| [Inventory Management](questions/09-inventory-management/README.md) | [Code](solutions/09-inventory-management/solution.hpp) / [design](solutions/09-inventory-management/design.md) | Observer |

## One interview-sized attempt

| Time | Focus |
| --- | --- |
| 0–5 min | Clarify the small workflow and assumptions. |
| 5–15 min | Name responsibilities, ownership and pattern participants. |
| 15–40 min | Implement the core workflow in straightforward code. |
| 40–50 min | Test normal, boundary and failure cases. |
| 50–60 min | Discuss one change and its tradeoffs. |

The base prompts define the small contract. Follow-ups such as persistence, payment, expiration, async delivery and distributed coordination are discussion items rather than extra plumbing in the core. Most examples are single-threaded; the Rate Limiter keeps one mutex per policy because atomic admission is central to that problem.

These interview-sized editions replace the earlier broader APIs. The current prompts, starters and tests are paired with the current solutions; previous editions remain in Git history.

## Run a reference solution

```bash
bash scripts/test.sh
bash scripts/test.sh 01-parking-lot
```

Or with CMake:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The direct compiler script enables UBSan by default. CMake offers `-DLLD_ENABLE_SANITIZERS=ON`. The CI exercises GCC, Clang and Apple Clang.

## Practice without opening the answer

Complete a question's `starter.hpp`, then run its matching tests:

```bash
bash scripts/test.sh 01-parking-lot practice
```

Or configure CMake with `-DLLD_PRACTICE_EXAMPLE=01-parking-lot`, build `practice_tests`, and run that test target. Substitute another example directory for other problems.

The starters intentionally throw TODO errors. Practice targets compile the starter instead of the solution; passing reference tests does not validate an unfinished attempt.

## OOP and patterns

The [pattern guide](docs/design-patterns.md) maps the actual interfaces and collaborators and includes all 23 GoF patterns as a reference catalog. Patterns in the main table are implemented in the core; additional patterns in follow-up discussions are extensions.

For each design, explain which behavior varies, which object protects an invariant, and who owns or borrows each collaborator.

## Quizzes

- [OOP foundations](quizzes/questions.md) → [answers](quizzes/solutions.md)
- [Example scenarios](quizzes/examples-questions.md) → [answers](quizzes/examples-solutions.md)
- [Design patterns](quizzes/design-patterns-questions.md) → [answers](quizzes/design-patterns-solutions.md)

## Study plan

| Week | Examples | Focus |
| --- | --- | --- |
| 1 | Parking Lot, Connect Four | Encapsulation, numeric IDs and Strategy |
| 2 | Amazon Locker, Elevator | Allocation and dispatch responsibilities |
| 3 | File System, Logging Service | Composite, Observer, Adapter and lifetimes |
| 4 | Movie Ticket Booking, Inventory | Whole-request validation and lifecycle rules |
| 5 | Rate Limiter; revisit a previous example | Algorithm substitution and atomic admission |
| 6 | Timed mocks | Clear explanation and one follow-up change |

## Navigation

[Questions](questions/README.md) · [Solutions](solutions/README.md) · [Quizzes](quizzes/README.md) · [Interview playbook](docs/interview-playbook.md) · [Review checklist](docs/review-checklist.md) · [Contributing](CONTRIBUTING.md)

The MIT license covers public reuse.
