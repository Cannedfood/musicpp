#pragma once

#include "../Scale.hpp"
#include "../WholeNote.hpp"

namespace music::sheet {

struct KeySignature {
	Accidental          accidental;
	std::array<bool, 7> accidentals;

	constexpr KeySignature() noexcept : KeySignature(C, major()) {}

	constexpr KeySignature(NoteValue base, Scale scale) noexcept :
		accidental(SHARP),
		accidentals{}
	{
		assert(scale.size() == 7 && "KeySignature() only accepts heptatonic scales");

		auto scaleNotes  = scale.notes(base);

		auto cmajor = major().notes(C);
		auto sharpMajor = cmajor.rotate(whole(scaleNotes.front(), SHARP));
		auto flatMajor  = cmajor.rotate(whole(scaleNotes.front(), FLAT));

		std::array<bool, 7> flats{false};
		std::array<bool, 7> sharps{false};
		int num_flats = 0, num_sharps = 0;
		for(size_t i = 0; i < 7; i++){
			flats[i] = flatMajor[i] != scaleNotes[i];
			num_flats += flats[i]?1:0;
			sharps[i] = sharpMajor[i] != scaleNotes[i];
			num_sharps += sharps[i]?1:0;
		}

		if(num_flats < num_sharps) {
			this->accidental  = FLAT;
			this->accidentals = flats;
		}
		else {
			this->accidental  = SHARP;
			this->accidentals = sharps;
		}
	}

	constexpr std::array<Accidental, 7> signature() const noexcept {
		std::array<Accidental, 7> result{};
		for(auto note : WholeNoteValues()) {
			result[note] = accidentals[note]? accidental : NATURAL;
		}
		return result;
	}

	constexpr WholeNote wholeNote(Note n) const noexcept { return WholeNote(whole(n.value), n.octave); }

	bool operator==(KeySignature const& other) const noexcept { return accidentals == other.accidentals; }
	bool operator!=(KeySignature const& other) const noexcept { return !((*this) == other); }
};

} // namespace music::sheet
