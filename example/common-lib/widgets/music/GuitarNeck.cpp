#include "GuitarNeck.hpp"

namespace example::common {

static float _transform    (int   v, float off, float scale) noexcept { return v * scale + off; }
static int   _inv_transform(float v, float off, float scale) noexcept { return std::floor((v - off) / scale); }

Point GuitarNeckPainter::min() const noexcept {
	return offset;
}
Point GuitarNeckPainter::max() const noexcept {
	return offset + size();
}
Size  GuitarNeckPainter::size() const noexcept {
	return fretSize * Size(endFret - startFret, tuning.size());
}

float GuitarNeckPainter::notePosition(int fret) const noexcept {
	float fretPos = fretPosition(fret);
	return fret == 0? fretPos : fretPos - fretSize.w / 2;
}
Point GuitarNeckPainter::notePosition(int string, int fret) const noexcept {
	return {
		notePosition(fret),
		stringPosition(string) + fretSize.h / 2
	};
}

float GuitarNeckPainter::fretPosition(int fret) const noexcept { return _transform(fret, offset.x, fretSize.w); }
int   GuitarNeckPainter::fretAt      (float x)  const noexcept { return _inv_transform(x, offset.x, fretSize.w); }

float GuitarNeckPainter::stringPosition(int string) const noexcept { return _transform(string, offset.y, fretSize.h); }
int   GuitarNeckPainter::stringAt(float y)          const noexcept { return _inv_transform(y, offset.y, fretSize.h); }

std::pair<Note, int> GuitarNeckPainter::noteAt(Point p) const noexcept {
	int string = stringAt(p.y);
	return { tuning.at(string) + fretAt(p.x), string };
}
std::pair<int, int>  GuitarNeckPainter::positionAt(Point p) const noexcept {
	return { stringAt(p.y), fretAt(p.y) };
}

void GuitarNeckPainter::paintNeck(Canvas& canvas, Color background, Color stringColor, Color fretColor) const noexcept {
	Point min = this->min();
	Point max = this->max();

	// Draw frets
	for(int fret = startFret; fret <= endFret; fret++) {
		float x = fretPosition(fret);
		canvas.moveTo({x, min.y}).lineTo({x, max.y});
	}
	canvas.stroke(fretColor);

	// Draw strings
	for(size_t i = 0; i < tuning.size(); i++) {
		float y = stringPosition(i) + fretSize.h / 2;
		canvas.moveTo({min.x, y}).lineTo({max.x, y});
	}
	canvas.stroke(stringColor);
}
void GuitarNeckPainter::paintNote(
	Canvas& canvas,
	int string, int fret, std::string_view text,
	Color outlineColor, Color innerColor, Color textColor) const noexcept
{
	Point p = notePosition(string, fret);

	canvas
	.circle(p, fretSize.h / 2)
	.fillAnd(innerColor)
	.stroke(outlineColor);

	if(!text.empty()) {
		canvas
		.fontAlign(AlignCenter, AlignMiddle)
		.font("bold")
		.fontSize(std::ceil(fretSize.h * .55f))
		.fillColor(textColor)
		.text(text, p);
	}
}
void GuitarNeckPainter::paintNote(Canvas& canvas,
		int string, Note note, std::string_view text,
		Color outlineColor, Color innerColor, Color textColor) const noexcept
{
	paintNote(canvas, string, note - tuning.at(string), text, outlineColor, innerColor, textColor);
}

GuitarNeck::GuitarNeck(guitar::Tuning tuning, guitar::Chord chord, int bars) noexcept :
	tuning(tuning),
	chord(chord),
	bars(bars)
{
	if(this->bars < 0) {
		this->bars = std::max(12, chord.range(tuning).second + 1);
	}
}

GuitarNeckPainter GuitarNeck::painter() const noexcept {
	return GuitarNeckPainter(
		0,
		fretSize,
		tuning,
		0, bars
	);
}

Constraint GuitarNeck::measure(Canvas &canvas, Constraint const& impose) noexcept {
	return Constraint::exactly(painter().size()).clamp(impose);
}
void GuitarNeck::paint(Canvas &canvas) noexcept {
	GuitarNeckPainter painter = this->painter();

	painter.paintNeck(canvas, Black, White, LightGray);
	for(size_t string = 0; string < tuning.size(); string++) {
		for(WholeNote n = WholeNote::Floor(tuning.at(string));
		    n < WholeNote::Floor(tuning.at(string)) + 8;
		    n += 1)
		{
			painter.paintNote(canvas, string, n, to_string(n.value), White, Black, LightGray);
		}
	}
}

void GuitarNeck::nextChord() {
	NoteValue    baseNote    = E;
	music::Chord targetChord = {RELATIVE, {min3rd, maj3rd, min3rd}};

	while(
		chord.nextChord(tuning, baseNote, targetChord, bars) && // Regen if we didn't roll over and...
		( // Regen when any of these
			chord.lowestNote().value != baseNote ||
			!chord.probablyPlayable(tuning)
		)
	);
}

/*
void GuitarNeck::_paintString(Canvas& canvas, int string) const noexcept {
	float y = _stringPosition(string);
	float x = _barPosition(0);

	canvas
	.moveTo({x,y})
	.lineTo({size().w, y})
	.stroke(Gray);

	canvas
	.font("bold", barSize.h / 2)
	.fontAlign(NVG_ALIGN_LEFT, NVG_ALIGN_MIDDLE)
	.fillColor(primary)
	.text(to_string(tuning.at(string).value), {0, y});

	for(WholeNote n = WholeNote::Floor(tuning.at(string));
	    n < WholeNote::Floor(tuning.at(string)) + 8;
	    n += 1)
	{
		_paintNote(canvas, string, Note(n), White);
	}

	// _paintNote(canvas, string, chord[string], White, nullptr);

	// if(tracedNote.first == string) {
	// 	_paintNote(canvas, string, tuning.at(string) + tracedNote.second, Green, nullptr);
	// }
}

void GuitarNeck::_paintNote(Canvas& canvas, int string, Note note, Color color, std::string_view label) const noexcept {
	if(label.empty()) {
		auto relativeTo = chord.lowestNote();
		if(relativeTo != Note::Null())
			label = to_string((Interval) (std::abs(note - relativeTo) % octave)).data();
		else
			label = to_string(note.value).data();
	}

	float y = _stringPosition(string);

	if(note == Note::Null()) {
		canvas
		.font("bold", barSize.h)
		.fontAlign(NVG_ALIGN_CENTER, NVG_ALIGN_MIDDLE)
		.fillColor(color)
		.text("X", {_barPosition(0), y});
	}
	else {
		float x = _notePosition(note - tuning.at(string));

		canvas
		.circle({x, y}, barSize.h / 2 - 2)
		.fillAnd(color.complement())
		.stroke(color);

		canvas
		.font("bold", barSize.h / 2)
		.fontAlign(NVG_ALIGN_CENTER, NVG_ALIGN_MIDDLE)
		.fillColor(color)
		.text(label, {x, y});
	}
}
 */

} // namespace example::common
