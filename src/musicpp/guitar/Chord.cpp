#include "Chord.hpp"

namespace music::guitar {

bool Chord::hasImpossibleStretches(Tuning const& tuning) const noexcept {
	constexpr float y_scale = 1 / 7.f;
	constexpr float x_scale = 1;

	constexpr float max_range = 4;

	for(int ay = 0; ay < (int)size(); ay++) {
		if(at(ay) == tuning.at(ay)) continue; // Open string
		if(at(ay) == Note::Null()) continue;
		for(size_t by = ay + 1; by < size(); by++) {
			if(at(by) == tuning.at(by)) continue; // Open string
			if(at(by) == Note::Null()) continue;
			int ax = at(ay) - tuning.at(ay);
			int bx = at(by) - tuning.at(by);

			float dx = (ax - bx) * x_scale;
			float dy = (by - ay) * y_scale;

			if((dx*dx + dy*dy) > max_range*max_range) {
				return true;
			}
		}
	}

	return false;
}

bool Chord::probablyPlayable(Tuning const& tuning) const noexcept {
	return !hasImpossibleStretches(tuning); // TODO: impossible barres, impossible mutes (0X0X00)
}

bool Chord::incrementString(Tuning const& tuning, NoteValue baseNote, music::Chord const& notes, int string, unsigned bars) noexcept {
	for(;;) {
		if(at(string) == Note::Null()) {
			at(string) = tuning.at(string);
		}
		else {
			at(string) += 1;
		}

		if((at(string) - tuning.at(string)) > (int)bars) {
			at(string) = Note::Null();
			return true; // Rollover
		}

		if(notes.notes[(at(string) - baseNote).value]) {
			return false; // Next note, no rollover
		}
	}
}

bool Chord::containsAll(Tuning const& tuning, NoteValue baseNote, std::bitset<12> notes) noexcept {
	for(auto& note : *this) {
		if(note != Note::Null()) {
			notes.reset((note - baseNote).value);
		}
	}
	return notes.none();
}

Note Chord::lowestNote() const noexcept {
	Note result = Note::Null();
	for(auto& note : *this) {
		if(result == Note::Null() || (note != Note::Null() && note < result))
			result = note;
	}
	return result;
}

std::pair<int, int> Chord::range(Tuning const& tuning) const noexcept {
	int min = 1e7, max = -1;
	for(size_t string = 0; string < size(); string++) {
		if(at(string) != Note::Null()) {
			int bar = at(string) - tuning.at(string);
			min = std::min(min, bar);
			max = std::max(max, bar);
		}
	}
	if(max < 0) max = 0;
	if(min >= 1e7) min = max;
	return {min,max};
}

bool Chord::nextChord(Tuning const& tuning, NoteValue baseNote, music::Chord const& notes, unsigned bars) noexcept {
	resize(tuning.size());

	for(size_t i = 0; i < size(); i++) {
		if(!incrementString(tuning, baseNote, notes, i, bars)) {
			if(containsAll(tuning, baseNote, notes.notes))
				return true;
			else {
				i = 0; // Restart
			}
		}
	}

	return false;
}

} // namespace music::guitar
