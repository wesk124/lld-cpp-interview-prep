#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <thread>

using namespace lld::file_system;

int main() {
    test::Suite suite;
    suite.run("nested directories and idempotent mkdir", [] {
        FileSystem fs;
        fs.mkdir("/home/user");
        fs.mkdir("/home/user");
        CHECK(fs.list("/home").size() == 1);
        CHECK(fs.list("/home/user").empty());
    });
    suite.run("write and overwrite file", [] {
        FileSystem fs;
        fs.mkdir("/docs");
        fs.write_file("/docs/notes", "first");
        CHECK(fs.read_file("/docs/notes") == "first");
        fs.write_file("/docs/notes", "updated");
        CHECK(fs.read_file("/docs/notes") == "updated");
    });
    suite.run("lexically ordered directory listing", [] {
        FileSystem fs;
        fs.write_file("/z", "");
        fs.write_file("/a", "");
        const std::vector<std::string> expected{"a", "z"};
        CHECK(fs.list("/") == expected);
    });
    suite.run("file-directory boundary errors", [] {
        FileSystem fs;
        fs.mkdir("/dir");
        fs.write_file("/file", "value");
        EXPECT_THROW(std::logic_error, fs.write_file("/dir", ""));
        EXPECT_THROW(std::logic_error, fs.read_file("/dir"));
        EXPECT_THROW(std::logic_error, fs.mkdir("/file/child"));
        EXPECT_THROW(std::logic_error, fs.list("/file"));
    });
    suite.run("parents must exist", [] {
        FileSystem fs;
        EXPECT_THROW(std::out_of_range, fs.write_file("/missing/file", ""));
        EXPECT_THROW(std::out_of_range, fs.read_file("/missing"));
    });
    suite.run("safe removal and recursive subtree ownership", [] {
        FileSystem fs;
        fs.mkdir("/a/b");
        fs.write_file("/a/b/file", "data");
        EXPECT_THROW(std::logic_error, fs.remove("/a"));
        CHECK(fs.read_file("/a/b/file") == "data");
        fs.remove("/a", true);
        CHECK(fs.list("/").empty());
        EXPECT_THROW(std::invalid_argument, fs.remove("/", true));
    });
    suite.run("path normalization and validation", [] {
        FileSystem fs;
        fs.mkdir("//a///b/");
        fs.write_file("/a/b/file", "x");
        CHECK(fs.read_file("//a/b/file") == "x");
        EXPECT_THROW(std::invalid_argument, fs.mkdir("relative"));
        EXPECT_THROW(std::invalid_argument, fs.mkdir("/a/../b"));
        EXPECT_THROW(std::invalid_argument, fs.write_file("/a/file/", ""));
        EXPECT_THROW(std::invalid_argument, fs.read_file("/a/b/file/"));
    });
    suite.run("serialized concurrent writes", [] {
        FileSystem fs;
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] { fs.write_file("/file-" + std::to_string(i), "data"); });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(fs.list("/").size() == 8);
    });
    return suite.finish();
}
