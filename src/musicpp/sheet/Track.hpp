#pragma once

#include <variant>

#include "../Rhythm.hpp"
#include "../Note.hpp"

#include "KeySignature.hpp"

#include <vector>
#include <optional>

namespace music::sheet {

struct BarNote {
	Duration when;
	Duration duration;
	Note     note;
};

struct Bar {
	std::optional<KeySignature> staff;
	std::vector<BarNote>      notes;
};

struct Track {
	std::string instrument = "piano";
	std::vector<Bar> bars;
};

struct Sheet {
	std::vector<Track> tracks;
};

} // namespace music::sheet
