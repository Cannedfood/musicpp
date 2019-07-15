#pragma once

#include <musicpp.hpp>
#include <wg.hpp>

namespace music::arranger {

struct BarPainter {
	wg::Rect scale;

	score::Staff        staff = score::GrandStaff;
	score::KeySignature key   = {C, minor()};

	int paddingLines = 8;

	void drawStaff(wg::Canvas& canvas) noexcept;
	void drawKey(wg::Canvas& canvas) noexcept;

	void drawNote(wg::Canvas& canvas, float x, Note note) noexcept;
	void drawNote(wg::Canvas& canvas, float x, WholeNote note) noexcept;

	float noteHeight() const noexcept { return std::floor(scale.h / (staff.height() + paddingLines)); }
	float noteWidth()  const noexcept { return noteHeight() * 1.4f; }
	float lineOffset() const noexcept { return noteHeight() * (paddingLines / 2); }

	float line(Note note)   const noexcept { return line(key.wholeNote(note)); }
	float line(WholeNote n) const noexcept { return line(staff.lineof(n)); }
	float line(int line)    const noexcept { return (scale.h - (line * noteHeight() + lineOffset())); }
};

} // namespace music::arranger
