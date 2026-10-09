#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::file_system;

int main() {
    test::Suite suite;
    suite.run("Composite sums leaves and nested directories", [] {
        FileSystem filesystem;
        std::unique_ptr<Directory> docs(new Directory("docs"));
        CHECK(docs->add(std::unique_ptr<Node>(new File("notes", "abc"))));
        CHECK(filesystem.root().add(std::move(docs)));
        CHECK(filesystem.root().add(std::unique_ptr<Node>(new File("readme", "12345"))));
        const Node& tree = filesystem.root();
        CHECK(tree.size() == 8);
        CHECK(filesystem.size() == 8);
        CHECK(filesystem.root().child("docs")->size() == 3);
    });
    suite.run("lookup, sorted listing and duplicate names", [] {
        Directory directory("root");
        CHECK(directory.add(std::unique_ptr<Node>(new File("b", ""))));
        CHECK(directory.add(std::unique_ptr<Node>(new File("a", ""))));
        CHECK(!directory.add(std::unique_ptr<Node>(new File("a", "duplicate"))));
        CHECK(directory.names() == std::vector<std::string>({"a", "b"}));
        CHECK(directory.child("missing") == nullptr);
        const Directory& read_only = directory;
        CHECK(read_only.child("a")->name() == "a");
    });
    suite.run("file edits and subtree removal", [] {
        File file("notes", "abc");
        CHECK(file.read() == "abc");
        file.write("hello");
        CHECK(file.size() == 5);
        Directory root("root");
        std::unique_ptr<Directory> nested(new Directory("nested"));
        CHECK(nested->add(std::unique_ptr<Node>(new File("notes", "hello"))));
        CHECK(root.add(std::move(nested)));
        CHECK(root.remove("nested"));
        CHECK(root.size() == 0);
        CHECK(!root.remove("nested"));
    });
    suite.run("virtual destruction through owned Node", [] {
        class TrackedNode : public Node {
        public:
            explicit TrackedNode(bool& flag) : Node("tracked"), flag_(flag) {}
            ~TrackedNode() override { flag_ = true; }
            std::size_t size() const override { return 0; }
        private:
            bool& flag_;
        };
        bool destroyed = false;
        Directory root("root");
        CHECK(root.add(std::unique_ptr<Node>(new TrackedNode(destroyed))));
        CHECK(root.remove("tracked"));
        CHECK(destroyed);
    });
    return suite.finish();
}
