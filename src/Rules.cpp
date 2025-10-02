#include "Rules.h"
#include <algorithm>

static int force_advance_chain(Bases& b, int bases_to_advance) {
    int runs = 0;
    for (int step = 0; step < bases_to_advance; ++step) {
        if (b.occ[2]) { runs++; b.occ[2] = false; }
        if (b.occ[1]) { b.occ[2] = true; b.occ[1] = false; }
        if (b.occ[0]) { b.occ[1] = true; b.occ[0] = false; }
    }
    return runs;
}

void Rules::score_runs(GameState& g, int runs) {
    if (g.half == HalfInning::Top) g.score.away += runs; else g.score.home += runs;
}

void Rules::walk_force_advance(GameState& g) {
    int runs = 0;
    if (g.bases.occ[0] && g.bases.occ[1] && g.bases.occ[2]) runs += force_advance_chain(g.bases, 1);
    else if (g.bases.occ[0]) runs += force_advance_chain(g.bases, 1);
    g.bases.occ[0] = true;
    score_runs(g, runs);
    g.count.reset_for_new_batter();
}

void Rules::record_out(GameState& g) {
    g.count.outs++;
    g.count.reset_for_new_batter();
    if (g.count.outs >= 3) {
        g.count.outs = 0;
        g.bases = Bases{};
        g.half = (g.half == HalfInning::Top) ? HalfInning::Bottom : HalfInning::Top;
        if (g.half == HalfInning::Top) g.inning++;
    }
}

int Rules::advance_on_hit(GameState& g, PitchOutcome hit) {
    int runs = 0;
    auto adv = [&](int n){ runs += force_advance_chain(g.bases, n); };
    switch (hit) {
        case PitchOutcome::Single:  adv(1); g.bases.occ[0] = true; break;
        case PitchOutcome::Double_: adv(2); g.bases.occ[1] = true; break;
        case PitchOutcome::Triple:  adv(3); g.bases.occ[2] = true; break;
        case PitchOutcome::HomeRun:
            runs += (g.bases.occ[0]?1:0) + (g.bases.occ[1]?1:0) + (g.bases.occ[2]?1:0) + 1;
            g.bases = Bases{}; g.count.reset_for_new_batter(); score_runs(g, runs); return runs;
        default: break;
    }
    g.count.reset_for_new_batter(); score_runs(g, runs); return runs;
}

bool Rules::apply_pitch(GameState& g, PitchOutcome o) {
    switch (o) {
        case PitchOutcome::Ball:
            if (++g.count.balls >= 4) { walk_force_advance(g); return true; } return false;
        case PitchOutcome::CalledStrike:
        case PitchOutcome::SwingMiss:
            if (++g.count.strikes >= 3) { record_out(g); return true; } return false;
        case PitchOutcome::Foul:
            if (g.count.strikes < 2) g.count.strikes++; return false;
        case PitchOutcome::InPlayOut:
            record_out(g); return true;
        case PitchOutcome::Single:
        case PitchOutcome::Double_:
        case PitchOutcome::Triple:
        case PitchOutcome::HomeRun:
            (void)advance_on_hit(g, o); return true;
    }
    return false;
}

PitchOutcome Rules::sample_outcome(const GameState& g,
                                   const BatterProfile& b,
                                   const PitcherProfile& p,
                                   RNG& rng) {
    const int balls = g.count.balls, strikes = g.count.strikes;
    double swing_chance = b.swing_aggression;
    if (balls >= 2 && strikes == 0) swing_chance *= 0.8;
    if (strikes == 2) swing_chance *= 0.9;
    bool swing = rng.uniform01() < swing_chance;
    if (!swing) {
        double zone_bias = p.zone_rate + 0.1 * (strikes == 2 ? 0.5 : 0.0);
        bool in_zone = rng.uniform01() < std::clamp(zone_bias, 0.0, 0.95);
        return in_zone ? PitchOutcome::CalledStrike : PitchOutcome::Ball;
    }
    double contact = std::clamp(b.contact_skill + 0.05, 0.05, 0.95);
    bool makes_contact = rng.uniform01() < contact;
    if (!makes_contact) return PitchOutcome::SwingMiss;
    double foul_prob = 0.18 + (strikes == 2 ? 0.10 : 0.0);
    if (rng.uniform01() < foul_prob) return PitchOutcome::Foul;
    double p1B = 0.15 + 0.10*(1.0 - b.power);
    double p2B = 0.06 + 0.07*(b.power);
    double p3B = 0.01;
    double pHR = 0.04 + 0.10*(b.power);
    double pOUT= 1.0 - (p1B + p2B + p3B + pHR);
    int idx = rng.choose_by_cdf({pOUT, p1B, p2B, p3B, pHR});
    switch (idx) {
        case 0: return PitchOutcome::InPlayOut;
        case 1: return PitchOutcome::Single;
        case 2: return PitchOutcome::Double_;
        case 3: return PitchOutcome::Triple;
        case 4: return PitchOutcome::HomeRun;
        default: return PitchOutcome::InPlayOut;
    }
}

void Rules::next_batter(GameState& g) { g.count.reset_for_new_batter(); g.ball_in_play = false; }