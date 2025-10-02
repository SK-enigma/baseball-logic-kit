#include "Rules.h"
#include <iostream>

static void print_state(const GameState& g) {
    std::cout << "Inning " << g.inning << (g.half==HalfInning::Top?" (Top)":" (Bottom)")
              << "  Score A:" << g.score.away << " B:" << g.score.home << "\n";
    std::cout << "Count " << g.count.balls << "-" << g.count.strikes
              << "  Outs:" << g.count.outs
              << "  Bases:["
              << (g.bases.occ[0]?'1':'.')
              << (g.bases.occ[1]?'2':'.')
              << (g.bases.occ[2]?'3':'.') << "]\n";
}

int main() {
    GameState g;
    BatterProfile bat{ .swing_aggression=0.55, .contact_skill=0.55, .power=0.35 };
    PitcherProfile pit{ .zone_rate=0.60, .k_ability=0.50 };
    RNG rng(12345);

    std::cout << "== Baseball Logic Kit Demo ==\n";
    print_state(g);

    while (true) {
        auto outcome = Rules::sample_outcome(g, bat, pit, rng);
        bool endPA = Rules::apply_pitch(g, outcome);

        std::cout << "Pitch -> ";
        switch (outcome) {
            case PitchOutcome::Ball:         std::cout << "Ball"; break;
            case PitchOutcome::CalledStrike: std::cout << "CalledStrike"; break;
            case PitchOutcome::SwingMiss:    std::cout << "SwingMiss"; break;
            case PitchOutcome::Foul:         std::cout << "Foul"; break;
            case PitchOutcome::InPlayOut:    std::cout << "InPlayOut"; break;
            case PitchOutcome::Single:       std::cout << "Single"; break;
            case PitchOutcome::Double_:      std::cout << "Double"; break;
            case PitchOutcome::Triple:       std::cout << "Triple"; break;
            case PitchOutcome::HomeRun:      std::cout << "HomeRun"; break;
        }
        std::cout << (endPA ? "  [PA end]\n" : "\n");

        if (g.count.outs == 0 && g.half == HalfInning::Bottom && g.inning == 1) {
            print_state(g); break;
        }
        print_state(g);
    }

    std::cout << "== Demo end ==\n";
    return 0;
}