// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#pragma once

#include "Enums.hpp"

#include <cmath>
#include <tuple>

namespace music {

// Enums

enum Accidental : int8_t { FLAT    = -1, NATURAL = 0, SHARP   = 1, };

/// The pitch class without octave information
enum Chroma : int8_t {
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
	constexpr inline int negative_modulo(int a, int b) {
		int result = a % b;
		if(result < 0) result += b;
		return result;
	}
}

constexpr inline Chroma operator+ (Chroma  v, int interval) noexcept { return Chroma(detail::negative_modulo(int(v) + interval, 12)); }
constexpr inline Chroma operator- (Chroma  v, int interval) noexcept { return Chroma(detail::negative_modulo(int(v) - interval, 12)); }
constexpr inline Chroma operator+=(Chroma& v, int interval) noexcept { return v = v + interval; }
constexpr inline Chroma operator-=(Chroma& v, int interval) noexcept { return v = v - interval; }

constexpr inline
bool not_sharp_or_flat(Chroma note) {
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
	min3rd   = 3,
	maj3rd   = 4,
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
	perf12th = 19,
};
constexpr inline Interval operator- (Chroma a, Chroma b)   noexcept { return Interval(detail::negative_modulo(int(a) - int(b), 12)); }

struct Pitch {
	std::int16_t midi;

	constexpr inline
	explicit Pitch(int midi) :
		midi(midi)
	{}
	constexpr inline
	Pitch(Chroma val = C, int octave = 4) noexcept :
		midi(val + 12 * (octave + 1))
	{}

	constexpr inline int    octave() const noexcept { return midi / 12 - 1; }
	constexpr inline Chroma chroma() const noexcept { return Chroma(midi % 12); }

	friend constexpr inline Pitch& operator+=(Pitch& a, int interval) noexcept;
	friend constexpr inline Pitch& operator-=(Pitch& a, int interval) noexcept;

	constexpr bool operator==(Pitch const& other) const noexcept { return midi == other.midi; }
	constexpr bool operator!=(Pitch const& other) const noexcept { return midi != other.midi; }
	constexpr bool operator> (Pitch const& other) const noexcept { return midi >  other.midi; }
	constexpr bool operator>=(Pitch const& other) const noexcept { return midi >= other.midi; }
	constexpr bool operator< (Pitch const& other) const noexcept { return midi <  other.midi; }
	constexpr bool operator<=(Pitch const& other) const noexcept { return midi <= other.midi; }
};

constexpr inline
std::string_view to_string(Chroma value, Accidental hint = NATURAL) noexcept {
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
Chroma from_string(std::string_view s, Accidental alter = NATURAL) {
	if(s == "C")  return Chroma::C  + alter;
	if(s == "C#") return Chroma::Cs + alter;
	if(s == "Cb") return Chroma::B  + alter;
	if(s == "D")  return Chroma::D  + alter;
	if(s == "D#") return Chroma::Db + alter;
	if(s == "Db") return Chroma::Ds + alter;
	if(s == "E")  return Chroma::E  + alter;
	if(s == "E#") return Chroma::F  + alter;
	if(s == "Eb") return Chroma::Eb + alter;
	if(s == "F")  return Chroma::F  + alter;
	if(s == "F#") return Chroma::Fs + alter;
	if(s == "Fb") return Chroma::Fb + alter;
	if(s == "G")  return Chroma::G  + alter;
	if(s == "G#") return Chroma::Gs + alter;
	if(s == "Gb") return Chroma::Gb + alter;
	if(s == "A")  return Chroma::A  + alter;
	if(s == "A#") return Chroma::As + alter;
	if(s == "Ab") return Chroma::Ab + alter;
	if(s == "B")  return Chroma::B  + alter;
	if(s == "B#") return Chroma::C  + alter;
	if(s == "Bb") return Chroma::Bb + alter;
	throw std::runtime_error("Failed parsing Chroma");
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

// ** Frequency *******************************************************
inline double frequency(Pitch n, double A_freq = 440) noexcept;
// ** Mathematical operators *******************************************************
constexpr inline Pitch operator+(Pitch n, int interval) noexcept;
constexpr inline Pitch operator-(Pitch n, int interval) noexcept;
constexpr inline int operator-(Pitch a, Pitch b) noexcept;


// =============================================================
// == Inline implementation =============================================
// =============================================================

// ** Frequency *******************************************************

inline double frequency(Pitch n, double A_freq) noexcept {
	return pow(2, (n - Pitch{A, 3}) / 12.0) * A_freq;
}

// ** Mathematical operators *******************************************************

constexpr inline Pitch  operator+ (Pitch  n, int interval) noexcept { return Pitch(n.midi + interval); }
constexpr inline Pitch  operator- (Pitch  n, int interval) noexcept { return Pitch(n.midi - interval); }
constexpr inline Pitch& operator+=(Pitch& n, int interval) noexcept { return n = Pitch(n + interval); }
constexpr inline Pitch& operator-=(Pitch& n, int interval) noexcept { return n = Pitch(n - interval); }
constexpr inline int   operator- (Pitch a, Pitch b)        noexcept { return a.midi - b.midi; }

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
