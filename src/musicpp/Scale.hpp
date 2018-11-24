// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#pragma once

#include "Enums.hpp"
#include "Note.hpp"
#include "util/StaticVector.hpp"

#include <array>
#include <bitset>
#include <algorithm>

namespace music {

struct Scale : public util::StaticVector<Interval, 16> {
	template<size_t N> constexpr
	Scale(std::array<NoteValue, N> notes) noexcept {
		resize(notes.size() - 1);
		for(size_t i = 0; i < notes.size() - 1; i++){
			at(i) = notes[i+1] - notes[i];
		}
	}

	constexpr explicit
	Scale(StaticVector intervals) noexcept :
		StaticVector(intervals)
	{}

	constexpr Scale rotate(int n) const noexcept { return Scale(StaticVector::rotate(n)); }

	constexpr auto notes(NoteValue base) -> util::StaticVector<NoteValue, 16> {
		util::StaticVector<NoteValue, 16> result;
		result.push_back(base);
		for(size_t i = 0; i < size() - 1; i++) {
			base += at(i);
			result.push_back(base);
		}
		return result;
	}

	constexpr auto notes(Note base) -> util::StaticVector<Note, 16> {
		util::StaticVector<Note, 16> result;
		result.push_back(base);
		for(size_t i = 0; i < size() - 1; i++) {
			base += at(i);
			result.push_back(base);
		}
		return result;
	}
};

// Modes
constexpr Scale ionian()     noexcept { return std::array{ C, D, E, F, G, A, B, C }; }
constexpr Scale dorian()     noexcept { return ionian().rotate(1); }
constexpr Scale phrygian()   noexcept { return ionian().rotate(2); }
constexpr Scale lydian()     noexcept { return ionian().rotate(3); }
constexpr Scale mixolydian() noexcept { return ionian().rotate(4); }
constexpr Scale aeolian()    noexcept { return ionian().rotate(5); }
constexpr Scale locrian()    noexcept { return ionian().rotate(6); }

// Major + Minor
constexpr Scale major() noexcept { return ionian(); }
constexpr Scale minor() noexcept { return aeolian(); }
constexpr Scale harmonic_minor() noexcept { return std::array{ A, B, C, D, E, F, Gs, A }; }

} // namespace music

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
