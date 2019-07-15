#pragma once

#include <variant>

#include "../Rhythm.hpp"
#include "../Note.hpp"

#include "KeySignature.hpp"

#include <vector>
#include <optional>
#include <memory>

namespace music::score {

struct Section {
	Duration start, end;

	std::variant<
		TimeSignature,
		Bpm,
		KeySignature
	> data;
};

struct TrackNote {
	Note      note = {C,4};
	TimePoint start = {0};
	Duration  duration = Duration::quarter();
	float     dynamic = 1;
};

struct Track {
	std::vector<TrackNote> notes;

	std::string instrument;
};

struct Sheet {
	std::vector<std::shared_ptr<Section>> sections;
	std::vector<Track>                    tracks;

	template<class Callback>
	void sectionsIn(TimePoint start, TimePoint end, Callback&& callback) {
		for(auto& section : sections) {
			if(section->start <= start && section->end <= end) {
				callback(*section);
			}
		}
	}
};

} // namespace music::score
