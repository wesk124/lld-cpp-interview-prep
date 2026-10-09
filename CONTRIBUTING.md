# Contributing

Contributions, alternative designs, and explanations of tradeoffs are welcome.

## Repository layout

Each example pairs a question in `questions/` with a reference implementation in `solutions/`. The question includes a prompt, TODO starter, and test plan; the solution includes code, a design explanation, and behavioral tests. Quiz questions and their answers live in separate files under `quizzes/`.

The existing build targets select the TODO starter when `LLD_PRACTICE` is set and the reference implementation otherwise. Root CMake and `scripts/test.sh` list the available examples.

## Running the tests

The current build scripts use C++11. To run the reference tests:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The direct-build alternative is `bash scripts/test.sh`. For a practice attempt, use `bash scripts/test.sh <directory-name> practice`; the unfinished starters intentionally fail with TODO errors.

A contribution description can explain the design choices, example behavior, and validation performed.
