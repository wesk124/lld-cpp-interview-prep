#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <sstream>

using namespace lld::logging_service;

int main() {
    test::Suite suite;
    suite.run("severity filtering", [] {
        MemorySink sink;
        Logger logger;
        logger.add_sink(sink);
        logger.log(Level::debug, "hidden");
        logger.log(Level::info, "shown");
        CHECK(sink.records().size() == 1);
        CHECK(sink.records()[0].message == "shown");
    });
    suite.run("Observer fanout and unsubscribe", [] {
        MemorySink first, second;
        Logger logger;
        logger.add_sink(first); logger.add_sink(second);
        logger.log(Level::warning, "one");
        CHECK(first.records().size() == 1 && second.records().size() == 1);
        logger.remove_sink(second);
        logger.log(Level::error, "two");
        CHECK(first.records().size() == 2 && second.records().size() == 1);
    });
    suite.run("ostream Adapter", [] {
        std::ostringstream output;
        StreamSink sink(output);
        Logger logger;
        logger.add_sink(sink);
        logger.log(Level::warning, "check");
        CHECK(output.str() == "[WARNING] check\n");
    });
    suite.run("threshold changes without changing sinks", [] {
        MemorySink sink;
        Logger logger(Level::error);
        logger.add_sink(sink);
        logger.log(Level::info, "hidden");
        logger.set_minimum(Level::debug);
        logger.log(Level::debug, "shown");
        CHECK(sink.records().size() == 1);
    });
    return suite.finish();
}
