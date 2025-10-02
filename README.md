# Baseball Logic Kit (C++)

Deterministic, testable **rules engine** for baseball with a tiny console demo and unit tests.

## Build (Windows & Linux)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --output-on-failure --build-config Release
```