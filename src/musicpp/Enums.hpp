// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#pragma once

#include <string_view>
#include <array>
#include <stdexcept>

namespace music {

enum Accidental : int8_t {
	FLAT    = -1,
	NATURAL = 0,
	SHARP   = 1,
};

enum NoteValue : int8_t {
	C = 0,
	Cs = 1,
	D = 2,   Db = Cs,
	Ds = 3,
	E = 4,   Eb = Ds,
	F = 5,   Fb = E,
	Fs = 6,  Gb = Fs,
	G = 7,
	Gs = 8,  Ab = Gs,
	A = 9,
	As = 10, Bb = As,
	B = 11,
};

namespace detail {
	constexpr inline int negativeModulo(int a, int b) {
		int result = a % b;
		if(result < 0) result += b;
		return result;
	}
}

constexpr inline NoteValue operator+ (NoteValue  v, int interval) noexcept { return NoteValue(detail::negativeModulo(int(v) + interval, 12)); }
constexpr inline NoteValue operator- (NoteValue  v, int interval) noexcept { return NoteValue(detail::negativeModulo(int(v) - interval, 12)); }
constexpr inline NoteValue operator+=(NoteValue& v, int interval) noexcept { return v = v + interval; }
constexpr inline NoteValue operator-=(NoteValue& v, int interval) noexcept { return v = v - interval; }

enum WholeNoteValue : int8_t {
	NOTE_C = 0,
	NOTE_D = 1,
	NOTE_E = 2,
	NOTE_F = 3,
	NOTE_G = 4,
	NOTE_A = 5,
	NOTE_B = 6,
};

constexpr std::array<WholeNoteValue, 7> WholeNoteValues() noexcept { return {NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_B}; }

constexpr inline
NoteValue chromatic(WholeNoteValue note, Accidental accidental = NATURAL) noexcept {
	switch(note) {
		case NOTE_C: return C + accidental;
		case NOTE_D: return D + accidental;
		case NOTE_E: return E + accidental;
		case NOTE_F: return F + accidental;
		case NOTE_G: return G + accidental;
		case NOTE_A: return A + accidental;
		case NOTE_B: return B + accidental;
	}
}

constexpr inline
WholeNoteValue whole(NoteValue note, Accidental interpretAs = NATURAL) noexcept {
	switch(note) {
	case C:  return (interpretAs == SHARP)? NOTE_B : NOTE_C;
	case Cs: return (interpretAs == SHARP)? NOTE_C : NOTE_D;
	case D:  return NOTE_D;
	case Ds: return (interpretAs == SHARP)? NOTE_D : NOTE_E;
	case E:  return (interpretAs == FLAT)?  NOTE_F : NOTE_E;
	case F:  return (interpretAs == SHARP)? NOTE_E : NOTE_F;
	case Fs: return (interpretAs == SHARP)? NOTE_F : NOTE_G;
	case G:  return NOTE_G;
	case Gs: return (interpretAs == SHARP)? NOTE_G : NOTE_A;
	case A:  return NOTE_A;
	case As: return (interpretAs == SHARP)? NOTE_A : NOTE_B;
	case B:  return (interpretAs == FLAT)?  NOTE_C : NOTE_B;
	}
}

constexpr inline
bool is_whole(NoteValue note) {
	switch(note){
		case A: case B: case C: case D: case E: case F: case G:
			return true;
		default:
			return false;
	}
}

enum Interval : int {
	prime    = 0,
	min2nd   = 1,
	maj2nd   = 2,
	min3rd   = 3, m = 3,
	maj3rd   = 4, M = 4,
	perf4th  = 5,
	aug4th   = 6,
	dim5th   = 6,
	perf5th  = 7,
	aug5th   = 8,
	min6th   = 8,
	maj6th   = 9,
	min7th   = 10,
	maj7th   = 11,
	octave   = 12,
	min9th   = 13,
	maj9th   = 14,
	min10th  = 15,
	maj10th  = 16,
	perf11th = 17,
	aug11th  = 18,
	dim12th  = 18,
	pref12th = 19,
};
constexpr inline Interval operator- (NoteValue a, NoteValue b)   noexcept { return Interval(detail::negativeModulo(int(a) - int(b), 12)); }

constexpr inline
std::string_view to_string(NoteValue value, Accidental hint = NATURAL) noexcept {
	if(hint == NATURAL) hint = SHARP;
	switch(value) {
		case C:  return "C";
		case Cs: return hint==SHARP?"C#":"Db";
		case D:  return "D";
		case Ds: return hint==SHARP?"D#":"Eb";
		case E:  return "E";
		case F:  return "F";
		case Fs: return hint==SHARP?"F#":"Gb";
		case G:  return "G";
		case Gs: return hint==SHARP?"G#":"Ab";
		case A:  return "A";
		case As: return hint==SHARP?"A#":"Bb";
		case B:  return "B";
	}
}

constexpr inline
NoteValue from_string(std::string_view s, Accidental alter = NATURAL) {
	if(s == "C") return NoteValue::C   + alter;
	if(s == "C#") return NoteValue::Cs + alter;
	if(s == "Cb") return NoteValue::B  + alter;
	if(s == "D") return NoteValue::D   + alter;
	if(s == "D#") return NoteValue::Db + alter;
	if(s == "Db") return NoteValue::Ds + alter;
	if(s == "E") return NoteValue::E   + alter;
	if(s == "E#") return NoteValue::F   + alter;
	if(s == "Eb") return NoteValue::Eb   + alter;
	if(s == "F") return NoteValue::F   + alter;
	if(s == "F#") return NoteValue::Fs   + alter;
	if(s == "Fb") return NoteValue::Fb   + alter;
	if(s == "G") return NoteValue::G   + alter;
	if(s == "G#") return NoteValue::Gs   + alter;
	if(s == "Gb") return NoteValue::Gb   + alter;
	if(s == "A") return NoteValue::A   + alter;
	if(s == "A#") return NoteValue::As   + alter;
	if(s == "Ab") return NoteValue::Ab   + alter;
	if(s == "B") return NoteValue::B   + alter;
	if(s == "B#") return NoteValue::C   + alter;
	if(s == "Bb") return NoteValue::Bb   + alter;
	throw std::runtime_error("Failed parsing NoteValue");
}

constexpr inline
std::string_view to_string(WholeNoteValue value) noexcept {
	switch(value) {
		case NOTE_C: return "C";
		case NOTE_D: return "D";
		case NOTE_E: return "E";
		case NOTE_F: return "F";
		case NOTE_G: return "G";
		case NOTE_A: return "A";
		case NOTE_B: return "B";
	}
}

constexpr inline
std::string_view to_string(Interval ivl, Accidental hint = NATURAL) noexcept {
	if(hint == NATURAL) hint = SHARP;
	switch(ivl) {
		case prime:    return "1st";
		case min2nd:   return "m2nd";
		case maj2nd:   return "2nd";
		case min3rd:   return "m3rd";
		case maj3rd:   return "3rd";
		case perf4th:  return "4th";
		case aug4th:   return "aug. 4th";
		case perf5th:  return "5th";
		case aug5th:   return "aug. 5th";
		case maj6th:   return "6th";
		case min7th:   return "m7th";
		case maj7th:   return "7th";
		case octave:   return "octave";
		case min9th:   return "m9th";
		case maj9th:   return "9th";
		case min10th:  return "m10th";
		case maj10th:  return "10th";
		case perf11th: return "11th";
		case aug11th:  return "aug. 11th";
		default: return "?";
	}
}

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
