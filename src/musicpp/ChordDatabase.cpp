// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#include "./ChordDatabase.hpp"
#include "./util/Range.hpp"

namespace music {

ChordDatabase::ChordDatabase(bool autoInit)
{
	if(!autoInit) return;

	// -- Initialize with all kinds of chords --

	// Intervals
	add("prime", "",              IntervalSet::from_absolute({prime}));
	add("minor 2nd", "m2nd",      IntervalSet::from_absolute({min2nd}));
	add("major 2nd", "2nd",       IntervalSet::from_absolute({maj2nd}));
	add("minor 3rd", "m3rd",      IntervalSet::from_absolute({min3rd}));
	add("major 3rd", "3rd",       IntervalSet::from_absolute({maj3rd}));
	add("4th", "4th",             IntervalSet::from_absolute({perf4th}));
	add("diminished 5th", "dim5th", IntervalSet::from_absolute({dim5th}));
	add("5th", "5th",             IntervalSet::from_absolute({perf5th}));
	add("minor 6th", "m6th",      IntervalSet::from_absolute({min6th}));
	add("major 6th", "6th",       IntervalSet::from_absolute({maj6th}));
	add("minor 7th", "m7th",      IntervalSet::from_absolute({min7th}));
	add("major 7th", "7th",       IntervalSet::from_absolute({maj7th}));

	// "Normal" triads
	add("major triad",      "maj",  IntervalSet::from_absolute({maj3rd, perf5th}));
	add("minor triad",      "m",    IntervalSet::from_absolute({min3rd, perf5th}));

	// "Normal" 7ths
	add("major 7th",     "maj7", IntervalSet::from_absolute({maj3rd, perf5th, maj7th}));
	add("minor 7th",     "m7",   IntervalSet::from_absolute({min3rd, perf5th, min7th}));
	add("minor major 7th",  "m/maj7", IntervalSet::from_absolute({maj3rd, perf5th, min7th}));
	add("dominant 7th",  "dom7", IntervalSet::from_absolute({maj3rd, perf5th, min7th}));

	// Aug
	add("augmented triad", "aug",  IntervalSet::from_absolute({maj3rd, aug5th}));
	add("augmented 7th",   "aug7", IntervalSet::from_absolute({maj3rd, aug5th, maj7th}));

	// Dim/Flat 5
	add("diminished triad",    "dim",    IntervalSet::from_absolute({min3rd, dim5th}));
	add("minor 7th flat 5",    "m7b5",   IntervalSet::from_absolute({min3rd, dim5th, min7th}));
	add("dominant 7th flat 5", "dom7b5", IntervalSet::from_absolute({maj3rd, dim5th, min7th}));

	// Sus/Add
	add("suspended 2nd",    "sus2", IntervalSet::from_absolute({maj2nd, perf5th}));
	add("suspended 4th",    "sus4", IntervalSet::from_absolute({perf4th, perf5th}));
	add("added 2nd",        "add2", IntervalSet::from_absolute({maj2nd, maj3rd, perf5th}));
	add("added 4th",        "add4", IntervalSet::from_absolute({maj3rd, perf4th, perf5th}));
}

void ChordDatabase::add(std::string name, std::string shortname, IntervalSet intervals) noexcept {

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
