// Copyright (c) 2018 Benno Straub, licensed under the MIT license. (A copy can be found at the bottom of this file)

#pragma once

#include <type_traits>
#include <cstdint>

namespace music {

struct Duration;
constexpr inline Duration operator+(Duration a, Duration b);
constexpr inline Duration operator-(Duration a, Duration b);
constexpr inline double   operator/(Duration a, Duration b);
constexpr inline Duration operator*(Duration v, int n);
constexpr inline Duration operator*(int n,      Duration v);
constexpr inline Duration operator/(Duration v, int n);
constexpr inline Duration operator*(Duration v, unsigned n);
constexpr inline Duration operator*(unsigned n, Duration v);
constexpr inline Duration operator/(Duration v, unsigned n);
constexpr inline Duration operator*(Duration v, double d);
constexpr inline Duration operator*(double d,   Duration v);
constexpr inline Duration operator/(Duration v, double d);

struct Duration {
	using base = std::uint32_t;

	base value;

	constexpr static Duration whole()        { return sixtyfourth() * 64u; }
	constexpr static Duration half()         { return sixtyfourth() * 32u; }
	constexpr static Duration quarter()      { return sixtyfourth() * 16u; }
	constexpr static Duration eighth()       { return sixtyfourth() * 8u; }
	constexpr static Duration sixteenth()    { return sixtyfourth() * 4u; }
	constexpr static Duration thirtysecond() { return sixtyfourth() * 2u; }
	constexpr static Duration sixtyfourth()  { return {3 * 3 * 3 * 5 * 7 * 11}; }

	constexpr Duration triplets()         const { return *this * 2 / 3; }
	constexpr Duration quintuplets()      const { return *this * 4 / 5; }
	constexpr Duration septuplets()       const { return *this * 4 / 7; }

	constexpr double in_beats() const { return value / (double) quarter().value; }

	constexpr bool operator==(Duration const& other) const { return value == other.value; }
	constexpr bool operator!=(Duration const& other) const { return value != other.value; }
	constexpr bool operator>=(Duration const& other) const { return value >= other.value; }
	constexpr bool operator<=(Duration const& other) const { return value <= other.value; }
	constexpr bool operator> (Duration const& other) const { return value >  other.value; }
	constexpr bool operator< (Duration const& other) const { return value <  other.value; }
};

using TimePoint = Duration;

constexpr inline Duration operator*(Duration v, int n)      { return { v.value * (unsigned)n }; }
constexpr inline Duration operator*(int      n, Duration v) { return { v.value * (unsigned)n }; }
constexpr inline Duration operator/(Duration v, int n)      { return { v.value / (unsigned)n }; }
constexpr inline Duration operator*(Duration v, unsigned n) { return { v.value * n }; }
constexpr inline Duration operator*(unsigned n, Duration v) { return { v.value * n }; }
constexpr inline Duration operator/(Duration v, unsigned n) { return { v.value / n }; }
constexpr inline Duration operator*(Duration v, double d)   { return { (uint32_t)(v.value * d) }; }
constexpr inline Duration operator*(double   d, Duration v) { return { (uint32_t)(v.value * d) }; }
constexpr inline Duration operator/(Duration v, double d)   { return { (uint32_t)(v.value / d) }; }

constexpr inline Duration operator+(Duration a, Duration b) { return { a.value + b.value }; }
constexpr inline Duration operator-(Duration a, Duration b) { return { a.value - b.value }; }
constexpr inline double   operator/(Duration a, Duration b) { return a.value / (double)b.value; }

struct TimeSignature {
	unsigned count;
	Duration base;

	constexpr TimeSignature(unsigned enm = 4, unsigned denom = 4) :
		count(enm),
		base(Duration::whole() / denom)
	{}

	constexpr Duration measure() const { return base * count; }
};

struct Bpm {
	double beatsPerMinute = 120;
};

} // namespace music

/*
 Copyright (c) 2018 Benno Straub

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
