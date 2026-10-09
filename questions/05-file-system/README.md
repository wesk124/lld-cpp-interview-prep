# File System

A 45–60-minute OOP exercise. Main pattern: **Composite**.

## Core interview contract

1. Create a tree of named files and directories in memory.
2. Add/remove/find a direct child and list names in lexical order; reject duplicate sibling names.
3. Compute size through Node: a file returns its content size and a directory sums child sizes recursively.
4. A directory exclusively owns its children; removing it destroys its subtree.

## Scope and assumptions

Single-threaded tree model. Navigation uses directory objects and direct child names. POSIX path parsing, host files, links and permissions are follow-ups, not part of this core exercise. Names are nonempty; size is text byte count.

## Interviewee TODOs

- [ ] Define the shared Node interface and a virtual destructor.
- [ ] Implement File as the leaf and Directory as the Composite.
- [ ] Use unique_ptr for child ownership and recursive polymorphic size calculation.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 05-file-system practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=05-file-system` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add path-based lookup and validation as a separate workflow.
- Add Visitor for exports/reports, or Command for undoable edits.

[After your attempt: design](../../solutions/05-file-system/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
