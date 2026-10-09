# Contributing

Alternative designs and concise explanations of tradeoffs are welcome.

## Layout

Each numbered example pairs a question with a TODO `starter.hpp` and test checklist, plus a reference `solution.hpp`, `tests.cpp` and design explanation. Quiz questions and answers are separate.

The examples focus on a 45–60-minute core workflow and an identifiable pattern. Advanced features can be described as follow-ups.

## Test commands

```bash
bash scripts/test.sh
bash scripts/test.sh 01-parking-lot practice
```

The current scripts use C++11. CMake is also available:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Practice targets compile the question starter instead of including the answer. A contribution description can explain the core contract, responsibilities, ownership, pattern roles and validation performed.
