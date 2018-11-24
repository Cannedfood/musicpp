#include "Bar.hpp"

namespace music::arranger {

Constraint Bar::measure(Canvas &canvas, Constraint impose) noexcept {
	float height = preferredNoteHeight * (staff.height() + paddingLines);
	return impose.with_min({ height, height }).with_pref({ height, height * 1.5f }).with_max({ Inf, Inf }).clamp(impose);
}

void Bar::paint(Canvas& canvas) noexcept {
	drawStaff(canvas);
	drawKey(canvas);

	float x = 0;
	for(Note note : major().notes(C)) {
		drawNote(canvas, x+=noteHeight(), note);
	}
}

void Bar::drawNote(Canvas& canvas, float x, Note note) const noexcept {
	drawNote(canvas, x, key.wholeNote(note));
}

void Bar::drawNote(Canvas& canvas, float x, WholeNote note) const noexcept {
	Point at = { x, line(note) };

	canvas
	.ellipse(at, .5f * Size(noteWidth(), noteHeight()))
	.fill(White);

	canvas
	.fillColor(Black)
	.font("bold", noteHeight() * .9f)
	.fontAlign(NVG_ALIGN_MIDDLE, NVG_ALIGN_CENTER)
	.text(to_string(note.value), at);
}

void Bar::drawStaff(Canvas& canvas) const noexcept {
	for(auto& l : staff.lines) {
		float y = line(l.index);
		canvas.moveTo({0,        y})
		      .lineTo({size().w, y});
	}
	canvas.stroke(White);
}

void Bar::drawKey(Canvas& canvas) const noexcept {
	canvas
	.font("bold", noteHeight() * 2)
	.fontAlign(NVG_ALIGN_LEFT, NVG_ALIGN_MIDDLE)
	.fillColor(White);

	auto signature = key.signature();
	for(auto n = staff.min; n <= staff.max; n += 1) {
		switch(signature[n.value]) {
		case Accidental::FLAT:    canvas.text("b", {0, line(n)}); break;
		case Accidental::SHARP:   canvas.text("#", {0, line(n)}); break;
		case Accidental::NATURAL: break;
		}
	}
}

} // namespace music::arranger
