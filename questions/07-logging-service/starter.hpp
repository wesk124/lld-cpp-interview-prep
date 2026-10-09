#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace logging_service {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

enum class Level { debug, info, warning, error };
struct Record { Level level; std::string message; };
class Sink {
public:
    virtual ~Sink() {}
    virtual void write(const Record& record) = 0;
};
class MemorySink : public Sink {
public:
    void write(const Record& /*record*/) override { todo("retain record values"); }
    const std::vector<Record>& records() const { todo("return records"); }
};
class StreamSink : public Sink {
public:
    explicit StreamSink(std::ostream& /*stream*/) { todo("borrow output stream"); }
    void write(const Record& /*record*/) override { todo("adapt ostream to Sink"); }
};
class Logger {
public:
    explicit Logger(Level /*minimum*/ = Level::info) { todo("store severity threshold"); }
    void add_sink(Sink& /*sink*/) { todo("register a non-owning Observer"); }
    void remove_sink(Sink& /*sink*/) { todo("unsubscribe sink"); }
    void set_minimum(Level /*minimum*/) { todo("change severity threshold"); }
    void log(Level /*level*/, const std::string& /*message*/) { todo("filter and notify every sink"); }
};

} // namespace logging_service
} // namespace lld
