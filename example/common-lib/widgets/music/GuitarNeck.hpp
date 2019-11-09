#pragma once

#include <wg.hpp>
#include <musicpp.hpp>

namespace example::common {

using namespace wg;
using namespace music;

struct GuitarNeckPainter {
	int startFret = 0;
	int endFret   = 12;

	Size  fretSize;
	Point offset;

	guitar::Tuning tuning;

	GuitarNeckPainter(Point offset, Size fretSize, guitar::Tuning tuning, int startFret, int endFret) :
		startFret(startFret),
		endFret(endFret),
		fretSize(fretSize),
		offset(offset),
		tuning(tuning)
	{}

	std::pair<Note, int> noteAt(Point p) const noexcept;
	std::pair<int, int>  positionAt(Point p) const noexcept;

	Point min() const noexcept;
	Point max() const noexcept;
	Size  size() const noexcept;

	float notePosition(int fret) const noexcept;
	Point notePosition(int string, int fret) const noexcept;

	float fretPosition(int fret) const noexcept;
	int   fretAt      (float x)  const noexcept;

	float stringPosition(int string) const noexcept;
	int   stringAt      (float y)    const noexcept;

	void paintNeck(Canvas& canvas,
		Color background,
		Color stringColor,
		Color fretColor) const noexcept;
	void paintNote(Canvas& canvas,
		int string, int fret, std::string_view text,
		Color outlineColor, Color innerColor, Color textColor) const noexcept;
	void paintNote(Canvas& canvas,
		int string, Note note, std::string_view text,
		Color outlineColor, Color innerColor, Color textColor) const noexcept;
};

class GuitarNeck : public Widget {
public:
	guitar::Tuning tuning;
	guitar::Chord  chord;

	int bars = 8;

	Size fretSize = Size{80, 30};
	std::pair<int, int> tracedNote = { -1, 0 };

	Color background = Black;
	Color primary    = White;
	Color secondary  = LightGray;
	Color highlight  = Red;

	GuitarNeck(guitar::Tuning tuning = guitar::Standard, guitar::Chord chord = {}, int bars = -1) noexcept;

	Constraint measure(Canvas &canvas, Constraint const& impose) noexcept override;

	GuitarNeckPainter painter() const noexcept;

	void paint(Canvas &canvas) noexcept override;

	void nextChord();
};

} // namespace example::common
