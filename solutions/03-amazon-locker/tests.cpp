#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

using namespace lld::amazon_locker;

std::function<std::string()> codes() {
    std::shared_ptr<int> next(new int(0));
    return [next] { return "code-" + std::to_string(++*next); };
}

int main() {
    test::Suite suite;
    suite.run("choose smallest compatible free slot", [] {
        Locker locker({{"L", Size::large}, {"S", Size::small}, {"M", Size::medium}}, codes());
        const auto first = locker.deposit("one", Size::small, TimePoint{}, std::chrono::hours(1));
        CHECK(first && first->slot_id == "S");
        const auto second = locker.deposit("two", Size::medium, TimePoint{}, std::chrono::hours(1));
        CHECK(second && second->slot_id == "M");
    });
    suite.run("capacity and duplicate package", [] {
        Locker locker({{"S", Size::small}}, codes());
        CHECK(!locker.deposit("large", Size::large, TimePoint{}, std::chrono::hours(1)));
        CHECK(locker.deposit("one", Size::small, TimePoint{}, std::chrono::hours(1)));
        CHECK(!locker.deposit("one", Size::small, TimePoint{}, std::chrono::hours(1)));
        CHECK(!locker.deposit("two", Size::small, TimePoint{}, std::chrono::hours(1)));
    });
    suite.run("single-use pickup code", [] {
        Locker locker({{"S", Size::small}}, codes());
        auto assignment = locker.deposit("package", Size::small, TimePoint{}, std::chrono::hours(1));
        CHECK(assignment);
        CHECK(!locker.pickup("wrong-code", TimePoint{}));
        const auto package = locker.pickup(assignment->pickup_code, TimePoint{});
        CHECK(package && *package == "package");
        CHECK(!locker.pickup(assignment->pickup_code, TimePoint{}));
        CHECK(locker.available_slots() == 1);
    });
    suite.run("expired packages require physical collection", [] {
        Locker locker({{"S", Size::small}}, codes());
        auto assignment = locker.deposit("old", Size::small, TimePoint{}, std::chrono::seconds(10));
        CHECK(assignment);
        CHECK(!locker.pickup(assignment->pickup_code, TimePoint{} + std::chrono::seconds(10)));
        CHECK(locker.available_slots() == 0);
        auto expired = locker.collect_expired(TimePoint{} + std::chrono::seconds(10));
        CHECK(expired.size() == 1 && expired[0] == "old");
        CHECK(locker.available_slots() == 1);
        CHECK(locker.collect_expired(TimePoint{} + std::chrono::seconds(10)).empty());
    });
    suite.run("code collision leaves free slot unchanged", [] {
        Locker locker({{"A", Size::small}, {"B", Size::small}}, [] { return "same"; });
        CHECK(locker.deposit("one", Size::small, TimePoint{}, std::chrono::hours(1)));
        EXPECT_THROW(std::logic_error, locker.deposit("two", Size::small, TimePoint{}, std::chrono::hours(1)));
        CHECK(locker.available_slots() == 1);
    });
    suite.run("configuration and timestamp validation", [] {
        EXPECT_THROW(std::invalid_argument, Locker({{"A", Size::small}, {"A", Size::large}}, codes()));
        Locker locker({{"S", Size::small}}, codes());
        EXPECT_THROW(std::invalid_argument, locker.deposit("", Size::small, TimePoint{}, std::chrono::hours(1)));
        EXPECT_THROW(std::invalid_argument, locker.deposit("x", Size::small, TimePoint{}, std::chrono::seconds(0)));
        CHECK(locker.deposit("x", Size::small, TimePoint{} + std::chrono::seconds(5), std::chrono::hours(1)));
        EXPECT_THROW(std::invalid_argument, locker.pickup("anything", TimePoint{}));
    });
    suite.run("concurrent deposits cannot overfill one slot", [] {
        Locker locker({{"S", Size::small}}, codes());
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                if (locker.deposit(std::to_string(i), Size::small, TimePoint{}, std::chrono::hours(1))) { ++accepted; }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 1);
    });
    return suite.finish();
}
