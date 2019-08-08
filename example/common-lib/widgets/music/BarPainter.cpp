#include "BarPainter.hpp"

namespace example::common {

void BarPainter::drawStaff(wg::Canvas& canvas) noexcept {
	for (auto& l : staff.lines) {
		float y = line(l.index);
		canvas.moveTo({0, y}).lineTo({scale.w, y});
	}
	canvas.stroke(wg::White);
}

void BarPainter::drawKey(wg::Canvas& canvas) noexcept {
	canvas.font("bold", noteHeight() * 2)
			.fontAlign(wg::AlignLeft, wg::AlignMiddle)
			.fillColor(wg::White);

	auto signature = key.signature();
	for (auto n = staff.min; n <= staff.max; n += 1) {
		switch (signature[n.value]) {
			case Accidental::FLAT:
				canvas.text("b", {0, line(n)});
				break;
			case Accidental::SHARP:
				canvas.text("#", {0, line(n)});
				break;
			case Accidental::NATURAL:
				break;
		}
	}
}

void BarPainter::drawNote(wg::Canvas& canvas, float x, Note note) noexcept {
	drawNote(canvas, x, key.wholeNote(note));
}

void BarPainter::drawNote(wg::Canvas& canvas, float x, WholeNote note) noexcept {
	wg::Point at = {x, line(note)};

	canvas.ellipse(at, .5f * wg::Size(noteWidth(), noteHeight())).fill(wg::White);

	canvas.fillColor(wg::Black)
			.font("bold", noteHeight() * .9f)
			.fontAlign(wg::AlignCenter, wg::AlignMiddle)
			.text(to_string(note.value), at);
}

}  // namespace example::common
