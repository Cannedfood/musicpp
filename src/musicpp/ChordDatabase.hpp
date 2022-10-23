// Copyright 2018 Benno Straub, Licensed under the MIT License, a copy can be found at the bottom of this file.

#pragma once

#include "./IntervalSet.hpp"

#include <unordered_map>
#include <string_view>

namespace music {

/// Saves data about chords, there are too many to stick them into an enum.
class ChordDatabase {
	struct Entry {
		std::string_view name;
		std::string_view shortName;
		unsigned         root;
		unsigned         inversion;
		IntervalSet		 intervals;
	};
public:
	std::vector<Entry> entries;

	ChordDatabase(bool autoInit = true);

	void add(std::string name, std::string shortname, IntervalSet intervals) noexcept;
};

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
