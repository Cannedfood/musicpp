// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#pragma once

#include <bitset>
#include <unordered_map>

#include "Enums.hpp"
#include "Note.hpp"
#include "util/StaticVector.hpp"

namespace music {

enum ChordFormat {
	ABSOLUTE,
	RELATIVE,
	NOTES
};

class Chord {
public:
	std::bitset<12> notes;
	util::StaticVector<unsigned, 16> absoluteIntervals;

	template<class Range = std::initializer_list<unsigned>> constexpr
	Chord(ChordFormat format, Range&& range) noexcept {
		notes[0] = true;
		switch(format) {
			case NOTES: {
				unsigned lowestNote = ~0u;
				for(auto& note : range) {
					lowestNote = std::min(lowestNote, note);
				}
				for(auto& note : range) {
					notes[Note(note - lowestNote).value] = true;
				}
			} break;
			case ABSOLUTE: {
				for(auto& interval : range) {
					notes[Note(interval).value] = true;
				}
			} break;
			case RELATIVE: {
				unsigned index = 0;
				for(auto& interval : range) {
					index += interval;
					notes[Note(index).value] = true;
				}
			} break;
		}
	}

	unsigned apply(unsigned* notes, unsigned base = C) const noexcept;

	std::string to_string() const noexcept;
	std::string to_string(unsigned base) const noexcept;

	bool operator==(Chord const& other) const noexcept { return notes == other.notes; }
};

} // namespace music

namespace std {
template<>
struct hash<music::Chord> {
	size_t operator()(music::Chord const& chord) const noexcept {
		return std::hash<std::bitset<12>>()(chord.notes);
	}
};
} // namespace std

// The MIT License (MIT)

// Copyright (c) 2018 Benno Straub

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
