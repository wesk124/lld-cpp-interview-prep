# Contributing

Contributions should teach design reasoning, not just add code. All code targets **C++17**, uses the standard library, and must compile with GCC and Clang/Apple Clang. Do not introduce C++20 features or `bits/stdc++.h`.

## Adding a problem

Add matching numbered directories under `questions/` and `solutions/`:

- Question: README with clear contracts, TODO-based `starter.hpp`, and `test_plan.md`.
- Solution: design explanation, focused C++17 implementation, and behavioral tests.
- Tests: include `starter.hpp` when `LLD_PRACTICE` is set; otherwise include the reference code.
- Register the reference target and practice directory in root CMake and `scripts/test.sh`.
- Update both indexes and add scenario questions with separate answers.

Use `common/test_support.hpp` rather than `assert`, so checks also run in release builds. Tests must not depend on wall-clock sleeps. Add contention tests for shared-state invariants without assuming thread scheduling order.

## Verify before submitting

```bash
cmake -S . -B build -DLLD_ENABLE_SANITIZERS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Without CMake: `bash scripts/test.sh`. Also verify that the starter compiles independently with `bash scripts/test.sh <directory-name> practice`; TODO failures are expected until completed.

Explain which source of change an abstraction isolates, which state each lock protects, and what the example intentionally excludes. Never claim production security, durability, or race freedom solely from passing sample tests.
