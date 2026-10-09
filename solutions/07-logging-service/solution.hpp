#pragma once

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

namespace lld {
namespace logging_service {

enum class Level { debug, info, warning, error };
struct Record { Level level; std::string message; };

// Observer: sinks receive records published by Logger.
class Sink {
public:
    virtual ~Sink() {}
    virtual void write(const Record& record) = 0;
};

class MemorySink : public Sink {
public:
    void write(const Record& record) override { records_.push_back(record); }
    const std::vector<Record>& records() const { return records_; }
private:
    std::vector<Record> records_;
};

// Adapter: expose an existing ostream through the Sink interface.
class StreamSink : public Sink {
public:
    explicit StreamSink(std::ostream& stream) : stream_(stream) {}
    void write(const Record& record) override {
        const char* labels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
        stream_ << '[' << labels[static_cast<int>(record.level)] << "] " << record.message << '\n';
    }
private:
    std::ostream& stream_;
};

class Logger {
public:
    explicit Logger(Level minimum = Level::info) : minimum_(minimum) {}
    void add_sink(Sink& sink) { sinks_.push_back(&sink); }
    void remove_sink(Sink& sink) {
        sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), &sink), sinks_.end());
    }
    void set_minimum(Level minimum) { minimum_ = minimum; }
    void log(Level level, const std::string& message) {
        if (level < minimum_) return;
        Record record{level, message};
        for (Sink* sink : sinks_) sink->write(record);
    }
private:
    Level minimum_;
    std::vector<Sink*> sinks_; // Non-owning subscribers.
};

} // namespace logging_service
} // namespace lld
