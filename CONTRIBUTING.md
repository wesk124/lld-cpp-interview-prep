# Contributing

Contributions should help candidates reason about design rather than merely collect code.

## Adding a problem

Add a matching numbered directory under both `questions/` and `solutions/`:

- The question contains the prompt, constraints, follow-ups, TODO-based starter code, and test plan.
- The solution contains responsibilities, relationships, invariants, ownership, tradeoffs, focused C++20 code, behavior-oriented tests, and a `CMakeLists.txt`.

Keep each example small enough to discuss in a 45–60 minute interview. Prefer composition, explicit ownership, stable IDs, deterministic tests, and standard-library dependencies.

## Pull requests

Before opening a pull request:

```bash
cmake -S . -B build -DLLD_ENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Explain design tradeoffs in the pull request. If you introduce a pattern, state which source of change it isolates.
