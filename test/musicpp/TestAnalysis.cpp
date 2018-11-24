#include <catch2/catch.hpp>

#include <musicpp/analysis/Analysis.hpp>

using namespace music;

TEST_CASE("ChordDb correctly identifies chords", "[analysis]") {
	ChordDb db;
	CHECK(db.get(Chord(ABSOLUTE, { maj3rd, min3rd })).name != "Major");
}
