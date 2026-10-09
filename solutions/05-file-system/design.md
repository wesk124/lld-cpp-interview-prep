# File System: Reference Design

[Question](../../questions/05-file-system/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Node supplies polymorphic node kind and a virtual destructor. File owns text; Directory owns an ordered map of unique_ptr<Node> children (Composite-style hierarchy).

FileSystem owns the root directory by value. Node pointers are used only during lock-held traversal and never escape the API.

Path parsing normalizes repeated slashes and rejects relative paths and dot segments. The implementation is not a POSIX-compliant filesystem.

One mutex protects the tree. Directory/file checks precede casts, and unique_ptr destruction releases recursively owned descendants.

Traversal is O(D log B), with depth D and maximum sibling count B. Listing costs O(B) plus copied names; removing a subtree costs O(number of descendant nodes). Content copying adds O(bytes).

The coarse lock and recursive destruction are sufficient for interview scale. Deep untrusted trees need bounded depth, and durability/atomic rename require separate design work.


## Scope

In-memory text only. No host OS files are created/deleted by FileSystem. No links, permissions, append, rename, quotas, or persistence. list accepts directories only; file read/write paths must not end with a slash. Each public method is serialized; mkdir may partially complete on allocation failure.

## Extend it yourself

- Add atomic rename and cycle prevention.
- Add permissions, size quotas, or append.
- Support symbolic links with bounded traversal depth.
