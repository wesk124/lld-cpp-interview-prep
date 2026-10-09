# C++ Low-Level Design Interview Prep

A public, code-first collection for practicing **object-oriented low-level design (LLD)** interviews: responsibilities, encapsulation, interfaces, ownership, polymorphism, and design patterns. The examples and build scripts use **C++11** and the standard library.

Each of the nine examples has a question with interviewee TODOs, an explained reference implementation, deterministic behavioral tests, and related quiz questions. These are focused interview exercises, not production-ready services.

## Questions and solutions

| Question / TODO starter | Solution | OOP focus | Pattern focus |
| --- | --- | --- | --- |
| [Parking Lot](questions/01-parking-lot/README.md) | [Reference design](solutions/01-parking-lot/design.md) | Policy abstraction, ownership, allocation and checkout invariants | Strategy; Decorator (exercise) |
| [Connect Four](questions/02-connect-four/README.md) | [Reference design](solutions/02-connect-four/design.md) | Encapsulated board, turn and lifecycle rules | State, Strategy, Command (exercises) |
| [Amazon Locker](questions/03-amazon-locker/README.md) | [Reference design](solutions/03-amazon-locker/design.md) | Composed slot/session values and injected behavior | Strategy via callable; Adapter, Observer (exercises) |
| [Elevator](questions/04-elevator/README.md) | [Reference design](solutions/04-elevator/design.md) | Car/bank responsibilities and dispatch polymorphism | Strategy; State (exercise) |
| [File System](questions/05-file-system/README.md) | [Reference design](solutions/05-file-system/design.md) | Node hierarchy, recursive ownership and public API | Composite-style hierarchy, Facade; Visitor (exercise) |
| [Movie Ticket Booking](questions/06-movie-ticket-booking/README.md) | [Reference design](solutions/06-movie-ticket-booking/design.md) | Seat ownership, hold lifecycle and service coordination | State, Strategy, Adapter (exercises) |
| [Logging Service](questions/07-logging-service/README.md) | [Reference design](solutions/07-logging-service/design.md) | Sink interfaces, polymorphic delivery and shared lifetimes | Adapter, Observer-style fanout; Decorator (exercise) |
| [Rate Limiter](questions/08-rate-limiter/README.md) | [Reference design](solutions/08-rate-limiter/design.md) | Encapsulated per-client state and atomic admission | Strategy, Decorator (exercises) |
| [Inventory Management](questions/09-inventory-management/README.md) | [Reference design](solutions/09-inventory-management/design.md) | Stock values, reservation lifecycle and invariants | State, Strategy, Observer (exercises) |

## OOP and design patterns

The [design-pattern guide](docs/design-patterns.md) maps patterns to actual classes in all nine examples and includes the complete 23-pattern GoF catalog. Each reference design explains its OOP responsibilities and pattern choices; each question includes discussion TODOs for exploring alternatives.

Patterns marked **exercise** are follow-ups rather than implemented features. The guide also distinguishes enum-based state machines from the State pattern and algorithms such as token bucket and LOOK from design patterns.

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
- [OOP and design-pattern quiz](quizzes/design-patterns-questions.md) → [answers](quizzes/design-patterns-solutions.md)

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
- `docs/`: [OOP and design patterns](docs/design-patterns.md), [interview playbook](docs/interview-playbook.md), and [review rubric](docs/review-checklist.md)
- `templates/`: reusable requirements and retrospective notes

Contributions and alternative designs are welcome: see [CONTRIBUTING.md](CONTRIBUTING.md). Public reuse is covered by the [MIT license](LICENSE).
