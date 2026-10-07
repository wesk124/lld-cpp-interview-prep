# File System

Design a small in-memory file system supporting directories and text files.

## Interview contract

1. Support recursive mkdir, write/overwrite file, read file, lexical directory listing, and removal.
2. A file's parent must already exist. Do not traverse through a file or overwrite a directory.
3. Accept absolute paths only; normalize repeated slashes and reject dot/dot-dot segments.
4. Protect the root from deletion. Removing a nonempty directory requires recursive=true.
5. Return snapshots rather than borrowed node pointers and serialize public operations.

## Scope and assumptions

In-memory text only. No host OS files are created/deleted by FileSystem. No links, permissions, append, rename, quotas, or persistence. list accepts directories only; file read/write paths must not end with a slash. Each public method is serialized; mkdir may partially complete on allocation failure.

## Interviewee TODOs

- [ ] Model a File/Directory tree with exclusive subtree ownership.
- [ ] Implement path parsing without consulting the host filesystem.
- [ ] Enforce file-versus-directory boundaries.
- [ ] Implement predictable error behavior for missing parents and invalid paths.
- [ ] Use RAII to release an entire subtree on recursive removal.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt (C++17)

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=05-file-system
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 05-file-system practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add atomic rename and cycle prevention.
2. Add permissions, size quotas, or append.
3. Support symbolic links with bounded traversal depth.

After your attempt: [design explanation](../../solutions/05-file-system/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
