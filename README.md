# C++ Low-Level Design Interview Prep

A practical, code-first path for preparing for low-level design (LLD) interviews in modern C++.

The repository focuses on what interviewers evaluate: clarifying requirements, assigning responsibilities, defining clean interfaces, explaining ownership, handling change, and implementing critical workflows with testable code.

## How to use this repository

For each problem:

1. Open the matching directory under `questions/` and avoid `solutions/`.
2. Spend 45–60 minutes designing aloud as if an interviewer were present.
3. Complete the marked `TODO` items and implement one important workflow.
4. Compare your approach with the matching reference solution.
5. Apply an extension from the problem's follow-up section.
6. Record tradeoffs and mistakes in a copy of `templates/retrospective.md`.

## Repository sections

- `questions/`: interview prompts and C++ starter code with explicit `TODO` tasks
- `solutions/`: explained, buildable reference implementations
- `quizzes/`: quick knowledge checks with separate answer keys
- `docs/`: interview method and review rubric
- `templates/`: reusable requirements and retrospective documents

## Six-week roadmap

| Week | Focus | Suggested problems |
| --- | --- | --- |
| 1 | OOP, SOLID, composition, RAII | Tic-tac-toe, parking lot |
| 2 | Strategy, Factory, Observer, State | Vending machine, elevator |
| 3 | Command, Decorator, Adapter, Chain | Logger, notification service |
| 4 | Domain modeling and workflows | Library, hotel, car rental |
| 5 | Thread safety and event-driven components | Cache, rate limiter, scheduler |
| 6 | Timed mocks and redesign drills | ATM, chess, expense sharing |

## Included now

- A parking-lot interview question with TODO-based starter code
- A complete parking-lot reference solution using C++20
- Explicit object ownership and stable identifiers
- Strategy-based pricing
- Thread-safe park and exit operations
- Deterministic tests using injected timestamps
- Reusable interview and retrospective templates
- GitHub Actions CI with strict compiler warnings

## Build and test

Requirements: CMake 3.20+ and a C++20 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Enable AddressSanitizer and UndefinedBehaviorSanitizer with GCC or Clang:

```bash
cmake -S . -B build -DLLD_ENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Interview workflow

A good 60-minute structure is:

- 5–8 minutes: clarify requirements and scope
- 8–10 minutes: identify entities, responsibilities, and invariants
- 10–15 minutes: define interfaces and relationships
- 15–20 minutes: implement the critical workflow
- 5 minutes: discuss tests, concurrency, failures, and extensions

See [the interview playbook](docs/interview-playbook.md) for the detailed approach and [the review checklist](docs/review-checklist.md) for a self-review rubric.

Start with the [C++ LLD foundations quiz](quizzes/questions.md), then check the [quiz solutions](quizzes/solutions.md).

## Contributions

New problems, alternative designs, tests, and explanations are welcome. See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT
