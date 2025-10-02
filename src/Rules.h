#pragma once
#include "GameState.h"
#include "RNG.h"

enum class PitchOutcome {
    Ball, CalledStrike, SwingMiss, Foul, InPlayOut, Single, Double_, Triple, HomeRun
};

struct BatterProfile { double swing_aggression=0.5, contact_skill=0.5, power=0.3; };
struct PitcherProfile { double zone_rate=0.6, k_ability=0.5; };

class Rules {
public:
    static bool apply_pitch(GameState& g, PitchOutcome o);
    static int  advance_on_hit(GameState& g, PitchOutcome hit);
    static PitchOutcome sample_outcome(const GameState& g,
                                       const BatterProfile& b,
                                       const PitcherProfile& p,
                                       RNG& rng);
    static void next_batter(GameState& g);
private:
    static void walk_force_advance(GameState& g);
    static void record_out(GameState& g);
    static void score_runs(GameState& g, int runs);
};