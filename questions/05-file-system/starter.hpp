#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace file_system {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

class Node {
public:
    explicit Node(std::string /*name*/) { todo("store node name"); }
    virtual ~Node() {}
    const std::string& name() const { todo("return node name"); }
    virtual std::size_t size() const = 0;
};
class File : public Node {
public:
    File(std::string name, std::string /*contents*/) : Node(std::move(name)) {
        todo("store file contents");
    }
    std::size_t size() const override { todo("return content byte count"); }
    const std::string& read() const { todo("return contents"); }
    void write(std::string /*contents*/) { todo("replace contents"); }
};
class Directory : public Node {
public:
    explicit Directory(std::string name) : Node(std::move(name)) {}
    bool add(std::unique_ptr<Node> /*node*/) { todo("take ownership if name is unique"); }
    bool remove(const std::string& /*name*/) { todo("remove child and owned subtree"); }
    Node* child(const std::string& /*name*/) { todo("find child or return nullptr"); }
    const Node* child(const std::string& /*name*/) const { todo("find a read-only child"); }
    std::vector<std::string> names() const { todo("return sorted child names"); }
    std::size_t size() const override { todo("sum sizes polymorphically: Composite"); }
};
class FileSystem {
public:
    FileSystem() { todo("own a root directory named /"); }
    Directory& root() { todo("return root"); }
    const Directory& root() const { todo("return read-only root"); }
    std::size_t size() const { todo("return root subtree size"); }
};

} // namespace file_system
} // namespace lld
