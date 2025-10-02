#pragma once
#include <array>
#include <string>

enum class HalfInning { Top, Bottom };

struct Bases { std::array<bool,3> occ {false,false,false}; };
struct Score { int away = 0; int home = 0; };

struct Count {
    int balls = 0, strikes = 0, outs = 0;
    void reset_for_new_batter() { balls = 0; strikes = 0; }
};

struct GameState {
    int inning = 1;
    HalfInning half = HalfInning::Top;
    Score score {};
    Bases bases {};
    Count count {};
    bool ball_in_play = false;
    std::string batting_side() const { return half == HalfInning::Top ? "away" : "home"; }
};