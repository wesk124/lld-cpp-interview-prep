#pragma once
#include <chrono>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace lld {
namespace logging_service {
using TimePoint = std::chrono::system_clock::time_point;
enum class Level { debug = 0, info = 1, warning = 2, error = 3 };
struct Record { TimePoint timestamp; Level level; std::string message; };
struct Delivery { std::size_t delivered; std::size_t failed; };
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class Sink {
public:
    virtual ~Sink() = default;
    virtual void write(const Record& record) = 0;
};
class MemorySink final : public Sink {
public:
    void write(const Record& /*record*/) override { todo("store complete records under a lock"); }
    std::vector<Record> records() const { todo("return a synchronized value snapshot"); }
};
class StreamSink final : public Sink {
public:
    explicit StreamSink(std::ostream& /*stream*/) { todo("retain a non-owning stream reference"); }
    void write(const Record& /*record*/) override { todo("format [LEVEL] message and detect stream failure"); }
};
class Logger {
public:
    explicit Logger(Level /*minimum*/ = Level::info) { todo("initialize a validated threshold"); }
    void add_sink(std::shared_ptr<Sink> /*sink*/) { todo("register a shared-lifetime sink"); }
    void set_minimum(Level /*minimum*/) { todo("update configuration safely"); }
    Delivery log(Level /*level*/, const std::string& /*message*/, TimePoint /*timestamp*/) {
        todo("filter, snapshot sinks, fan out without configuration lock, and isolate failures");
    }
private:
    // TODO: Document lifetime ownership and per-sink synchronization.
    // TODO: Do not promise a global record order across multiple producers/sinks.
};
}  // namespace logging_service
}  // namespace lld
