#pragma once

#include "../Note.hpp"
#include "../WholeNote.hpp"
#include "../util/StaticVector.hpp"

#include <algorithm>

namespace music::sheet {

enum StaffName {
	F_Clef, BassClef = F_Clef,
	C_Clef, AltoClef = C_Clef,
	G_Clef, TrebleClef = G_Clef,
	GrandStaff
};

struct Staff {
	struct Line {
		WholeNote note;
		int       index;
	};

	WholeNote min, max;
	util::StaticVector<Line, 32> lines;

	constexpr Staff() noexcept : min(), lines{} {}
	constexpr Staff(StaffName name) noexcept : Staff()
	{
		switch(name) {
			case F_Clef: *this = { {G,2}, {B,2}, {D,3}, {F,3}, {A,3} }; break;
			case C_Clef: *this = { {F,3}, {A,3}, {C,4}, {E,4}, {G,4} }; break;
			case G_Clef: *this = { {E,4}, {G,4}, {B,4}, {D,5}, {F,5} }; break;
			case GrandStaff: *this = Staff(BassClef) | Staff(TrebleClef); break;
		}
	}

	constexpr Staff(std::initializer_list<WholeNote> notes) noexcept : Staff()
	{
		min = WholeNote(NOTE_C, 500);
		max = WholeNote(NOTE_C, -500);
		for(auto& note : notes) {
			if(note < min) min = note;
			if(note > max) max = note;

			int line = note - min;
			lines.push_back({
				.note = note,
				.index = line
			});
		}
	}

	constexpr Staff operator|(Staff const& other) const noexcept {
		Staff result;
		result.min = std::min(this->min, other.min);
		result.max = std::max(this->max, other.max);
		result.lines.push_back(lines.begin(),       lines.end());
		result.lines.push_back(other.lines.begin(), other.lines.end());
		result.lines.sort_in_place([](Line const& a, Line const& b) { return a.note < b.note; });
		for(auto& line : result.lines) {
			line.index = line.note - min;
		}
		return result;
	}

	constexpr unsigned height() const noexcept { return max - min; }

	constexpr unsigned lineof(WholeNote n) const noexcept {
		return n - min;
	}
};

} // namespace music::sheet
