add_library(parking_lot
    src/parking_lot.cpp
)

target_include_directories(parking_lot PUBLIC include)
target_link_libraries(parking_lot PUBLIC lld_project_options)

add_executable(parking_lot_tests
    tests/parking_lot_test.cpp
)

target_link_libraries(parking_lot_tests PRIVATE parking_lot)

add_test(NAME parking_lot_tests COMMAND parking_lot_tests)
