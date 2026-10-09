# C++ Low-Level Design Interview Prep

A public, code-first collection for learning and practicing low-level design (LLD) interviews. The examples and build scripts use **C++11** and the standard library.

Each of the nine examples has a question with interviewee TODOs, an explained reference implementation, deterministic behavioral tests, and related quiz questions. These are focused interview exercises, not production-ready services.

## Questions and solutions

| Question / TODO starter | Solution | Main concepts |
| --- | --- | --- |
| [Parking Lot](questions/01-parking-lot/README.md) | [Reference design](solutions/01-parking-lot/design.md) | Ownership, Strategy, compatible allocation, atomic checkout |
| [Connect Four](questions/02-connect-four/README.md) | [Reference design](solutions/02-connect-four/design.md) | Board modeling, gravity, directional win detection, terminal states |
| [Amazon Locker](questions/03-amazon-locker/README.md) | [Reference design](solutions/03-amazon-locker/design.md) | Size-based allocation, injected codes, expiration, physical occupancy |
| [Elevator](questions/04-elevator/README.md) | [Reference design](solutions/04-elevator/design.md) | Door interlocks, LOOK scheduling, bank dispatch Strategy |
| [File System](questions/05-file-system/README.md) | [Reference design](solutions/05-file-system/design.md) | Composite hierarchy, unique ownership, path validation, traversal |
| [Movie Ticket Booking](questions/06-movie-ticket-booking/README.md) | [Reference design](solutions/06-movie-ticket-booking/design.md) | Atomic seat holds, state machine, expiry, idempotency |
| [Logging Service](questions/07-logging-service/README.md) | [Reference design](solutions/07-logging-service/design.md) | Sink interfaces, fanout, failure isolation, lock/lifetime boundaries |
| [Rate Limiter](questions/08-rate-limiter/README.md) | [Reference design](solutions/08-rate-limiter/design.md) | Token-bucket invariants, weighted requests, monotonic time, contention |
| [Inventory Management](questions/09-inventory-management/README.md) | [Reference design](solutions/09-inventory-management/design.md) | Stock invariants, multi-SKU transactions, idempotent lifecycle |

## Practice without seeing the answer

1. Pick a directory under `questions/`. Clarify the scope and state your invariants aloud.
2. Complete its `starter.hpp` TODOs. You may redesign the internals while keeping the public test contract.
3. Spend 45–60 minutes on the base design, then run the practice target.
4. Compare with `solutions/`, explain tradeoffs, and attempt a follow-up change.
5. Write a retrospective using [the template](templates/retrospective.md).

The starter headers compile, but intentionally throw TODO errors. **Reference tests passing does not mean your attempt passes**: practice targets explicitly compile the question header and never link the solution.

## Build and test all reference solutions

The CMake build uses C++11 and CMake 3.20+. A direct compiler script is also available below.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Enable AddressSanitizer and UndefinedBehaviorSanitizer on GCC/Clang:

```bash
cmake -S . -B build -DLLD_ENABLE_SANITIZERS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Without CMake, use the GCC/Clang direct-build script (UBSan enabled by default):

```bash
bash scripts/test.sh
bash scripts/test.sh 02-connect-four
```

## Run one interviewee attempt

Example: Connect Four. Substitute any catalog directory name.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=02-connect-four
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Or:

```bash
bash scripts/test.sh 02-connect-four practice
```

Use `CXX=clang++` to select Clang in the shell script. For compiler environments without UBSan, set `LLD_SANITIZERS=none`. Tests avoid wall-clock sleeps and include concurrent capacity/record-count cases where appropriate; sanitizer runs alone do not prove race freedom.

## Quizzes

- [C++ LLD foundations](quizzes/questions.md) → [answers](quizzes/solutions.md)
- [Nine-example scenario quiz](quizzes/examples-questions.md) → [answers](quizzes/examples-solutions.md)

Try answering before opening the explanations, then discuss the tradeoffs behind each choice.

## Six-week study plan

| Week | Practice | Focus |
| --- | --- | --- |
| 1 | Connect Four, Parking Lot | Ownership, invariants, value semantics |
| 2 | Amazon Locker, Elevator | Lifecycle modeling and allocation/scheduling policies |
| 3 | File System, Logging Service | Hierarchies, interfaces, lock/lifetime boundaries |
| 4 | Movie Ticket Booking, Inventory Management | Atomic multi-entity updates and idempotency |
| 5 | Rate Limiter; revisit concurrency tests | Monotonic time, contention, failure cases |
| 6 | Timed mocks and follow-ups | Communication, changing requirements, tradeoffs |

## Repository sections

- `questions/`: prompts, contracts, TODO-based runnable starter headers, and test plans
- `solutions/`: reference code, design explanations, and behavioral tests
- `quizzes/`: knowledge and scenario questions with separate answer keys
- `common/`: test harness and a small optional-value helper
- `docs/`: [interview playbook](docs/interview-playbook.md) and [review rubric](docs/review-checklist.md)
- `templates/`: reusable requirements and retrospective notes

Contributions and alternative designs are welcome: see [CONTRIBUTING.md](CONTRIBUTING.md). Public reuse is covered by the [MIT license](LICENSE).
