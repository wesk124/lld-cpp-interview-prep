#!/usr/bin/env bash
set -euo pipefail

# Run from any working directory.
root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$root_dir/out/direct-tests"
mkdir -p "$build_dir"
compiler="${CXX:-g++}"
selected="${1:-all}"
mode="${2:-solution}"
if (( $# > 2 )) || [[ "$mode" != "solution" && "$mode" != "practice" ]]; then
    echo "Usage: bash scripts/test.sh [all|example-directory] [solution|practice]" >&2
    exit 2
fi
if [[ "$mode" == "practice" && "$selected" == "all" ]]; then
    echo "Select one example for practice mode." >&2
    exit 2
fi
examples=(
    01-parking-lot 02-connect-four 03-amazon-locker 04-elevator
    05-file-system 06-movie-ticket-booking 07-logging-service
    08-rate-limiter 09-inventory-management
)
found=false
flags=(-std=c++11 -pthread -Wall -Wextra -Wpedantic -Wconversion -Wshadow
       -fno-omit-frame-pointer -I "$root_dir/common")
sanitizers="${LLD_SANITIZERS:-undefined}"
if [[ "$sanitizers" != "none" ]]; then
    flags+=("-fsanitize=$sanitizers" -fno-sanitize-recover=all)
fi
if [[ "$selected" == "all" ]]; then
    echo "Building optional-value helper tests"
    "$compiler" "${flags[@]}" "$root_dir/common/optional_tests.cpp" -o "$build_dir/optional-tests"
    "$build_dir/optional-tests"
fi
for example in "${examples[@]}"; do
    if [[ "$selected" != "all" && "$selected" != "$example" ]]; then continue; fi
    found=true
    sources=()
    includes=()
    if [[ "$example" == "01-parking-lot" ]]; then
        sources+=("$root_dir/solutions/$example/tests/parking_lot_test.cpp")
        if [[ "$mode" == "solution" ]]; then
            sources+=("$root_dir/solutions/$example/src/parking_lot.cpp")
            includes+=(-I "$root_dir/solutions/$example/include")
        fi
    else
        sources+=("$root_dir/solutions/$example/tests.cpp")
        if [[ "$mode" == "solution" ]]; then includes+=(-I "$root_dir/solutions/$example"); fi
    fi
    if [[ "$mode" == "practice" ]]; then
        includes+=(-DLLD_PRACTICE=1 -I "$root_dir/questions/$example")
    fi
    binary="$build_dir/$example-$mode"
    echo "Building $example ($mode, C++11)"
    "$compiler" "${flags[@]}" "${includes[@]}" "${sources[@]}" -o "$binary"
    "$binary"
done
if [[ "$found" == false ]]; then
    echo "Unknown example: $selected" >&2
    exit 2
fi
