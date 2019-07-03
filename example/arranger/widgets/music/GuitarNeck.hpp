#pragma once

#include <wg.hpp>
#include <musicpp.hpp>

namespace music::arranger {

using namespace wg;

class GuitarNeck : public Widget {
public:
	guitar::Tuning tuning;
	guitar::Chord  chord;

	int bars = 8;

	Size barSize = Size{80, 30};
	std::pair<int, int> tracedNote = { -1, 0 };

	Color background = Black;
	Color primary    = White;
	Color secondary  = LightGray;
	Color highlight  = Red;

	GuitarNeck(guitar::Tuning tuning = guitar::Standard, guitar::Chord chord = {}, int bars = -1) noexcept;

	Constraint measure(Canvas &canvas, Constraint const& impose) noexcept override;

	void paint(Canvas &canvas) noexcept override;

	void nextChord();
private:
	int   _barAt(float x) const noexcept;
	float _barPosition(int bar) const noexcept;
	float _notePosition(int bar) const noexcept;

	float _stringPosition(int string) const noexcept;
	int   _stringAt(float y) const noexcept;

	void _paintString(Canvas& canvas, int string) const noexcept;
	void _paintNote(Canvas& canvas, int string, Note note, Color color, std::string_view label = "") const noexcept;
};

} // namespace music::arranger
