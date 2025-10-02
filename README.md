# Baseball Logic Kit (C++)

Deterministic, testable **baseball rules engine** for simulations and prototypes—outs, innings, base states, and scoring—written in modern C++ with CMake, doctest, and a tiny CLI demo.

[![CI](https://github.com/SK-enigma/baseball-logic-kit/actions/workflows/ci.yml/badge.svg)](https://github.com/SK-enigma/baseball-logic-kit/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
<!-- Optional once you add coverage:
[![Coverage](https://img.shields.io/badge/coverage-pending-lightgrey.svg)](#)
-->

---

## Why this exists
- **Deterministic**: seedable RNG so sims are reproducible  
- **Small & focused**: core logic only; no UI framework  
- **Testable**: unit tests with doctest  
- **Portable build**: clean CMake presets and options  

---

## Quick Start

### Build (Release) & run tests
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build --output-on-failure --build-config Release


Run the demo
./build/bin/baseball_demo --seed 123


Example output (deterministic with the same seed):

T1: PA#1 -> Single (runner to 1B)
T1: PA#2 -> Groundout (1 out)
...
Score: Away 3 - Home 1 (End 5th)

<!-- Add this GIF later (optional but recommended) --> <!-- ![Console demo](docs/demo.gif) -->
Tiny API Example
#include "Rules.h"
#include "GameState.h"
#include "RNG.h"

int main() {
    bl::GameState g;     // inning=1, top; outs=0; empty bases; score 0-0
    bl::RNG rng{123};    // deterministic seed
    auto result = bl::Rules::plate_appearance(g, rng);
    // result.outcome -> e.g., bl::Outcome::Single
    // g now reflects new bases/outs/score
}


Concepts

GameState: inning, half, outs, base occupancy, score

Rules: pure functions mapping (state, rng) -> (outcome, new state)

RNG: seeded PRNG passed in (no hidden globals)

CMake Options
BUILD_DEMO   (ON/OFF) - build CLI demo (default: ON)
BUILD_TESTS  (ON/OFF) - build unit tests (default: ON)


Install as a library (optional):

cmake -S . -B build -DBUILD_TESTS=OFF -DBUILD_DEMO=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build --target install


Use from another project:

find_package(BaseballLogicKit CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE BaseballLogicKit::baseball_logic)

Tests

Run all tests:

ctest --test-dir build --output-on-failure


Core cases covered:

Single advances batter to 1B

Force advance from 1B on single

Three outs ends the half-inning

Same seed → same sequence
