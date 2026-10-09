#pragma once
#include <stdexcept>
#include <string>
#include <vector>
namespace lld {
namespace file_system {
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class FileSystem {
public:
    void mkdir(const std::string& /*path*/) { todo("create missing directories, rejecting file traversal"); }
    void write_file(const std::string& /*path*/, std::string /*contents*/) {
        todo("write or overwrite a file only when its parent directory exists");
    }
    std::string read_file(const std::string& /*path*/) const { todo("read file contents by value"); }
    std::vector<std::string> list(const std::string& /*path*/) const {
        todo("list directory children in lexical order");
    }
    void remove(const std::string& /*path*/, bool /*recursive*/ = false) {
        todo("protect the root; require recursive removal for nonempty directories");
    }
private:
    // TODO: Model File and Directory nodes with exclusive subtree ownership.
    // TODO: Parse absolute paths; normalize repeated slashes; reject . and ..
    // TODO: Keep all mutation confined to the in-memory tree, not the host OS.
};
}  // namespace file_system
}  // namespace lld
