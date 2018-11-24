#include "GuitarNeck.hpp"

namespace music::arranger {

GuitarNeck::GuitarNeck(guitar::Tuning tuning, guitar::Chord chord, int bars) noexcept :
	tuning(tuning),
	chord(chord),
	bars(bars)
{
	if(this->bars < 0) {
		this->bars = std::max(12, chord.range(tuning).second + 1);
	}
}

Constraint GuitarNeck::measure(Canvas &canvas, Constraint impose) noexcept {
	return Constraint::exactly(barSize * Size(bars, tuning.numStrings())).clamp(impose);
}
void GuitarNeck::paint(Canvas &canvas) noexcept {
	canvas.rect(size()).fillAnd(background).stroke(secondary);

	chord.resize(tuning.size());

	for(int bar = 0; bar < bars; bar++) {
		float x = _barPosition(bar);

		nvgMoveTo(canvas.context, x, 0);
		nvgLineTo(canvas.context, x, size().h);
		canvas.stroke(LightGray);

		canvas
		.font("default", barSize.h / 2)
		.fontAlign(NVG_ALIGN_CENTER, NVG_ALIGN_BOTTOM)
		.fillColor(primary)
		.text(std::to_string(bar).c_str(), {_notePosition(bar), 0});
	}

	for(size_t string = 0; string < tuning.numStrings(); string++) {
		_paintString(canvas, string);
	}

	canvas.stroke(White);
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

int   GuitarNeck::_barAt(float x) const noexcept { return std::round((x - 30 + barSize.w * .5f) / barSize.w); }
float GuitarNeck::_barPosition(int bar) const noexcept { return 30 + barSize.w * bar; }
float GuitarNeck::_notePosition(int bar) const noexcept {
	float bp = _barPosition(bar);
	if(bar != 0) {
		bp -= barSize.w / 2;
	}
	return bp;
}

float GuitarNeck::_stringPosition(int string) const noexcept { return size().h - ((barSize.h * string) + barSize.h * .5f); }
int   GuitarNeck::_stringAt(float y) const noexcept { return (size().h - y) / barSize.h; }

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
	/*
	_paintNote(canvas, string, chord[string], White, nullptr);

	if(tracedNote.first == string) {
		_paintNote(canvas, string, tuning.at(string) + tracedNote.second, Green, nullptr);
	}
	*/
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

} // namespace music::arranger
