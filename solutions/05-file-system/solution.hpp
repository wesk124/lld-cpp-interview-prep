#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace file_system {

// Composite: leaves and directories share the same size operation.
class Node {
public:
    explicit Node(std::string name) : name_(std::move(name)) {}
    virtual ~Node() {}
    const std::string& name() const { return name_; }
    virtual std::size_t size() const = 0;
private:
    std::string name_;
};

class File : public Node {
public:
    File(std::string name, std::string contents)
        : Node(std::move(name)), contents_(std::move(contents)) {}
    std::size_t size() const override { return contents_.size(); }
    const std::string& read() const { return contents_; }
    void write(std::string contents) { contents_ = std::move(contents); }
private:
    std::string contents_;
};

class Directory : public Node {
public:
    explicit Directory(std::string name) : Node(std::move(name)) {}

    bool add(std::unique_ptr<Node> node) {
        if (!node || children_.count(node->name()) != 0) return false;
        std::string key = node->name();
        children_.emplace(std::move(key), std::move(node));
        return true;
    }
    bool remove(const std::string& name) { return children_.erase(name) != 0; }
    Node* child(const std::string& name) {
        auto entry = children_.find(name);
        return entry == children_.end() ? nullptr : entry->second.get();
    }
    const Node* child(const std::string& name) const {
        auto entry = children_.find(name);
        return entry == children_.end() ? nullptr : entry->second.get();
    }
    std::vector<std::string> names() const {
        std::vector<std::string> result;
        for (const auto& entry : children_) result.push_back(entry.first);
        return result;
    }
    std::size_t size() const override {
        std::size_t total = 0;
        for (const auto& entry : children_) total += entry.second->size();
        return total;
    }

private:
    std::map<std::string, std::unique_ptr<Node>> children_;
};

class FileSystem {
public:
    FileSystem() : root_("/") {}
    Directory& root() { return root_; }
    const Directory& root() const { return root_; }
    std::size_t size() const { return root_.size(); }
private:
    Directory root_;
};

} // namespace file_system
} // namespace lld
