#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "Rules.h"

TEST_CASE("Ball/Strike count transitions and walk") {
    GameState g;
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK(g.count.balls == 1);
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK(g.bases.occ[0] == true);
    CHECK(g.count.balls == 0);
    CHECK(g.count.strikes == 0);
}

TEST_CASE("Foul ball at two strikes does not strike out") {
    GameState g;
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::CalledStrike));
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::CalledStrike));
    CHECK(g.count.strikes == 2);
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Foul));
    CHECK(g.count.strikes == 2);
}

TEST_CASE("Strikeout on third strike") {
    GameState g;
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::CalledStrike));
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::SwingMiss));
    CHECK(Rules::apply_pitch(g, PitchOutcome::CalledStrike));
    CHECK(g.count.outs == 1);
    CHECK(g.count.balls == 0);
    CHECK(g.count.strikes == 0);
}

TEST_CASE("Forced advance on walk with bases loaded scores a run") {
    GameState g;
    g.bases.occ = {true,true,true};
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK_FALSE(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK(Rules::apply_pitch(g, PitchOutcome::Ball));
    CHECK(g.score.away == 1);
    CHECK(g.bases.occ[0] == true);
    CHECK(g.bases.occ[1] == true);
    CHECK(g.bases.occ[2] == true);
}

TEST_CASE("Hit advancement: single moves runners one base, HR clears bases") {
    GameState g;
    g.bases.occ = {false,true,false};
    CHECK(Rules::apply_pitch(g, PitchOutcome::Single));
    CHECK(g.bases.occ[0] == true);
    CHECK(g.bases.occ[1] == false);
    CHECK(g.bases.occ[2] == true);

    g.bases.occ = {true,true,true};
    int before = g.score.away;
    CHECK(Rules::apply_pitch(g, PitchOutcome::HomeRun));
    CHECK(g.score.away == before + 4);
    CHECK(g.bases.occ[0] == false);
    CHECK(g.bases.occ[1] == false);
    CHECK(g.bases.occ[2] == false);
}

TEST_CASE("Three outs flip half-inning and reset bases") {
    GameState g;
    CHECK(Rules::apply_pitch(g, PitchOutcome::InPlayOut));
    CHECK(Rules::apply_pitch(g, PitchOutcome::InPlayOut));
    CHECK(Rules::apply_pitch(g, PitchOutcome::InPlayOut));
    CHECK(g.count.outs == 0);
    CHECK(g.half == HalfInning::Bottom);
    CHECK(g.inning == 1);
    CHECK_FALSE(g.bases.occ[0]);
    CHECK_FALSE(g.bases.occ[1]);
    CHECK_FALSE(g.bases.occ[2]);
}