#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace logging_service {

using TimePoint = std::chrono::system_clock::time_point;
enum class Level { debug = 0, info = 1, warning = 2, error = 3 };
struct Record {
    TimePoint timestamp;
    Level level;
    std::string message;
};
struct Delivery {
    std::size_t delivered;
    std::size_t failed;
};

class Sink {
public:
    virtual ~Sink() = default;
    virtual void write(const Record& record) = 0;
};

class MemorySink final : public Sink {
public:
    void write(const Record& record) override {
        std::lock_guard<std::mutex> lock(mutex_);
        records_.push_back(record);
    }
    std::vector<Record> records() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return records_;
    }
private:
    mutable std::mutex mutex_;
    std::vector<Record> records_;
};

class StreamSink final : public Sink {
public:
    explicit StreamSink(std::ostream& stream) : stream_(stream) {}
    void write(const Record& record) override {
        std::lock_guard<std::mutex> lock(mutex_);
        const char* labels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
        const int index = static_cast<int>(record.level);
        if (index < 0 || index > 3) { throw std::invalid_argument("invalid log level"); }
        stream_ << '[' << labels[index] << "] " << record.message << '\n';
        if (!stream_) { throw std::runtime_error("log stream write failed"); }
    }
private:
    std::mutex mutex_;
    std::ostream& stream_;  // Non-owning; caller must keep it alive.
};

class Logger {
public:
    explicit Logger(Level minimum = Level::info) : minimum_(minimum) { validate(minimum); }

    void add_sink(std::shared_ptr<Sink> sink) {
        if (!sink) { throw std::invalid_argument("sink cannot be null"); }
        std::lock_guard<std::mutex> lock(mutex_);
        sinks_.push_back(std::move(sink));
    }

    void set_minimum(Level minimum) {
        validate(minimum);
        std::lock_guard<std::mutex> lock(mutex_);
        minimum_ = minimum;
    }

    Delivery log(Level level, const std::string& message, TimePoint timestamp) {
        validate(level);
        std::vector<std::shared_ptr<Sink>> sinks;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (level < minimum_) { return Delivery{0, 0}; }
            sinks = sinks_;
        }  // Do not hold the logger's configuration lock while executing sink code.
        Record record{timestamp, level, message};
        Delivery result{0, 0};
        for (const auto& sink : sinks) {
            try {
                sink->write(record);
                ++result.delivered;
            } catch (...) {
                ++result.failed;  // One failing sink does not suppress other deliveries.
            }
        }
        return result;
    }

private:
    static void validate(Level level) {
        const int value = static_cast<int>(level);
        if (value < 0 || value > 3) { throw std::invalid_argument("invalid log level"); }
    }
    std::mutex mutex_;
    Level minimum_;
    std::vector<std::shared_ptr<Sink>> sinks_;
};

}  // namespace logging_service
}  // namespace lld
