# File System: Interview Design

[Question](../../questions/05-file-system/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Composite**.

| Object | Responsibility |
| --- | --- |
| `Node` | Common name and virtual size operation. |
| `File` | Leaf storing content bytes. |
| `Directory` | Composite owning child Node objects. |
| `FileSystem` | Own the root directory. |

Build the tree with Directory.add. Calling size through Node dispatches to File or Directory; directories recurse through the same interface.

## Ownership and scope

FileSystem owns its root, and directories own children through unique_ptr<Node>. root/child/read results borrow storage; they cannot outlive the owner or a removal/replacement of that storage. A rejected add consumes and destroys the supplied node.

Single-threaded tree model. Navigation uses directory objects and direct child names. POSIX path parsing, host files, links and permissions are follow-ups, not part of this core exercise. Names are nonempty; size is text byte count.

## Small usage example

Within the example's namespace:

```cpp
FileSystem filesystem;
std::unique_ptr<Directory> docs(new Directory("docs"));
docs->add(std::unique_ptr<Node>(new File("notes", "hello")));
filesystem.root().add(std::move(docs));
// filesystem.size() == 5
```

Subtree size/removal is O(nodes in the subtree). Direct-child lookup uses O(log children) map operations.

## Follow-up discussion

- Add path-based lookup and validation as a separate workflow.
- Add Visitor for exports/reports, or Command for undoable edits.

[Pattern map and catalog](../../docs/design-patterns.md)
