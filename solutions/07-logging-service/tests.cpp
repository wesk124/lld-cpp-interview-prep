#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <sstream>
#include <thread>

using namespace lld::logging_service;

class FailingSink final : public Sink {
public:
    void write(const Record&) override { throw std::runtime_error("sink failure"); }
};

int main() {
    test::Suite suite;
    suite.run("severity filter and injected timestamp", [] {
        Logger logger;
        auto memory = std::make_shared<MemorySink>();
        logger.add_sink(memory);
        CHECK(logger.log(Level::debug, "hidden", TimePoint{}).delivered == 0);
        CHECK(logger.log(Level::info, "visible", TimePoint{}).delivered == 1);
        const auto records = memory->records();
        CHECK(records.size() == 1 && records[0].message == "visible");
        CHECK(records[0].timestamp == TimePoint{});
    });
    suite.run("threshold can change at runtime", [] {
        Logger logger(Level::error);
        auto memory = std::make_shared<MemorySink>();
        logger.add_sink(memory);
        CHECK(logger.log(Level::warning, "first", TimePoint{}).delivered == 0);
        logger.set_minimum(Level::debug);
        CHECK(logger.log(Level::warning, "second", TimePoint{}).delivered == 1);
    });
    suite.run("fanout to multiple sinks", [] {
        Logger logger;
        auto a = std::make_shared<MemorySink>();
        auto b = std::make_shared<MemorySink>();
        logger.add_sink(a);
        logger.add_sink(b);
        auto result = logger.log(Level::error, "failure", TimePoint{});
        CHECK(result.delivered == 2 && result.failed == 0);
        CHECK(a->records().size() == 1 && b->records().size() == 1);
    });
    suite.run("failed sink does not suppress successful sinks", [] {
        Logger logger;
        auto memory = std::make_shared<MemorySink>();
        logger.add_sink(std::make_shared<FailingSink>());
        logger.add_sink(memory);
        auto result = logger.log(Level::error, "still delivered", TimePoint{});
        CHECK(result.failed == 1 && result.delivered == 1);
        CHECK(memory->records().size() == 1);
    });
    suite.run("stream formatting and failed stream detection", [] {
        std::ostringstream stream;
        StreamSink sink(stream);
        sink.write(Record{TimePoint{}, Level::warning, "message"});
        CHECK(stream.str() == "[WARNING] message\n");
        stream.setstate(std::ios::badbit);
        EXPECT_THROW(std::runtime_error, sink.write(Record{TimePoint{}, Level::info, "x"}));
    });
    suite.run("invalid sink and level", [] {
        Logger logger;
        EXPECT_THROW(std::invalid_argument, logger.add_sink(nullptr));
        EXPECT_THROW(std::invalid_argument, logger.set_minimum(static_cast<Level>(8)));
        CHECK(logger.log(Level::info, "no destinations", TimePoint{}).delivered == 0);
    });
    suite.run("concurrent producers preserve whole records", [] {
        Logger logger;
        auto memory = std::make_shared<MemorySink>();
        logger.add_sink(memory);
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                for (int j = 0; j < 20; ++j) {
                    logger.log(Level::info, std::to_string(i) + ":" + std::to_string(j), TimePoint{});
                }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(memory->records().size() == 160);
    });
    return suite.finish();
}
