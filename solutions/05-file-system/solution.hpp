#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace file_system {

class Node {
public:
    virtual ~Node() = default;
    virtual bool is_directory() const noexcept = 0;
};
class File final : public Node {
public:
    explicit File(std::string text) : contents(std::move(text)) {}
    bool is_directory() const noexcept override { return false; }
    std::string contents;
};
class Directory final : public Node {
public:
    bool is_directory() const noexcept override { return true; }
    std::map<std::string, std::unique_ptr<Node>> children;
};

// Owns an in-memory tree only: never reads or changes the host filesystem.
class FileSystem {
public:
    void mkdir(const std::string& path) {
        const auto parts = split(path);
        std::lock_guard<std::mutex> lock(mutex_);
        Directory* current = &root_;
        for (const auto& part : parts) {
            auto it = current->children.find(part);
            if (it == current->children.end()) {
                it = current->children.emplace(part, std::unique_ptr<Node>(new Directory())).first;
            }
            current = &as_directory(*it->second);
        }
    }

    void write_file(const std::string& path, std::string contents) {
        const auto parts = split(path);
        if (parts.empty() || path.back() == '/') { throw std::invalid_argument("file path required"); }
        std::lock_guard<std::mutex> lock(mutex_);
        auto& parent = parent_of(parts);
        const auto it = parent.children.find(parts.back());
        if (it != parent.children.end() && it->second->is_directory()) {
            throw std::logic_error("cannot overwrite a directory");
        }
        std::unique_ptr<Node> replacement(new File(std::move(contents)));
        if (it == parent.children.end()) {
            parent.children.emplace(parts.back(), std::move(replacement));
        } else {
            it->second = std::move(replacement);
        }
    }

    std::string read_file(const std::string& path) const {
        const auto parts = split(path);
        if (!parts.empty() && path.back() == '/') { throw std::invalid_argument("file path has trailing slash"); }
        std::lock_guard<std::mutex> lock(mutex_);
        const Node& node = resolve(parts);
        if (node.is_directory()) { throw std::logic_error("path is a directory"); }
        return static_cast<const File&>(node).contents;
    }

    std::vector<std::string> list(const std::string& path) const {
        const auto parts = split(path);
        std::lock_guard<std::mutex> lock(mutex_);
        const auto& directory = as_directory(resolve(parts));
        std::vector<std::string> names;
        for (const auto& child : directory.children) { names.push_back(child.first); }
        return names;  // std::map gives deterministic lexical ordering.
    }

    void remove(const std::string& path, bool recursive = false) {
        const auto parts = split(path);
        if (parts.empty()) { throw std::invalid_argument("cannot remove root"); }
        std::lock_guard<std::mutex> lock(mutex_);
        auto& parent = parent_of(parts);
        auto it = parent.children.find(parts.back());
        if (it == parent.children.end()) { throw std::out_of_range("path does not exist"); }
        if (path.back() == '/' && !it->second->is_directory()) {
            throw std::invalid_argument("file path has trailing slash");
        }
        if (it->second->is_directory() && !recursive &&
            !as_directory(*it->second).children.empty()) {
            throw std::logic_error("directory is not empty");
        }
        parent.children.erase(it);  // unique_ptr releases the entire owned subtree.
    }

private:
    static std::vector<std::string> split(const std::string& path) {
        if (path.empty() || path.front() != '/') { throw std::invalid_argument("absolute path required"); }
        std::vector<std::string> parts;
        std::size_t start = 1;
        while (start < path.size()) {
            const auto end = path.find('/', start);
            const auto part = path.substr(start, end == std::string::npos ? end : end - start);
            if (part == "." || part == "..") { throw std::invalid_argument("dot segments are not supported"); }
            if (!part.empty()) { parts.push_back(part); }
            if (end == std::string::npos) { break; }
            start = end + 1;
        }
        return parts;
    }
    static Directory& as_directory(Node& node) {
        if (!node.is_directory()) { throw std::logic_error("cannot traverse a file"); }
        return static_cast<Directory&>(node);
    }
    static const Directory& as_directory(const Node& node) {
        if (!node.is_directory()) { throw std::logic_error("path is not a directory"); }
        return static_cast<const Directory&>(node);
    }
    const Node& resolve(const std::vector<std::string>& parts) const {
        const Node* current = &root_;
        for (const auto& part : parts) {
            const auto& children = as_directory(*current).children;
            auto it = children.find(part);
            if (it == children.end()) { throw std::out_of_range("path does not exist"); }
            current = it->second.get();
        }
        return *current;
    }
    Directory& parent_of(const std::vector<std::string>& parts) {
        Directory* current = &root_;
        for (std::size_t i = 0; i + 1 < parts.size(); ++i) {
            auto it = current->children.find(parts[i]);
            if (it == current->children.end()) { throw std::out_of_range("parent does not exist"); }
            current = &as_directory(*it->second);
        }
        return *current;
    }
    mutable std::mutex mutex_;
    Directory root_;
};

}  // namespace file_system
}  // namespace lld
