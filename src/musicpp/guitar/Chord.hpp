#pragma once

#include "Tuning.hpp"

#include "../util/StaticVector.hpp"
#include "../Note.hpp"
#include "../Chord.hpp"

#include <set>

namespace music::guitar {

class Chord : public util::StaticVector<Note, 12> {
public:
	Chord() : StaticVector(Note::Null()) {}

	bool nextChord(Tuning const& tuning,       Chroma baseNote, music::Chord const& notes, unsigned bars = 16) noexcept;
	bool incrementString(Tuning const& tuning, Chroma baseNote, music::Chord const& notes, int string, unsigned bars = 16) noexcept;

	bool containsAll(Tuning const& tuning, Chroma baseNote, std::bitset<12> notes) noexcept;

	Note lowestNote() const noexcept;

	std::pair<int, int> range(Tuning const& tuning) const noexcept;

	bool probablyPlayable(Tuning const& tuning) const noexcept;
	bool hasImpossibleStretches(Tuning const& tuning) const noexcept;
};

} // namespace music::guitar
