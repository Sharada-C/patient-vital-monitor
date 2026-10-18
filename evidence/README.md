# Patient Vital-Signs Monitor — Test Evidence

This directory contains sample test and sanitizer output.

## Automated tests
- ctest_output.txt: CTest output for three test suites.
- Recorded result: 3 tests passed, 0 failed.

## Sanitizer runs
- asan_output.txt: AddressSanitizer output.
- ubsan_output.txt: UndefinedBehaviorSanitizer output.
- tsan_output.txt: ThreadSanitizer output.

The recorded runs completed with "Monitor shutdown complete."
The checked logs contained no matching sanitizer error or warning
messages. These results apply only to the recorded runs and do not
prove the absence of every possible defect.

## Reproduce the automated tests

Run from the repository root:

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

This is a POSIX-thread-based simulation for learning and testing,
not a medical device.
