#pragma once

#include <musicpp.hpp>
#include <wg.hpp>

namespace music::arranger {

using namespace wg;

class Bar : public Widget {
public:
	sheet::Staff        staff = sheet::GrandStaff;
	sheet::KeySignature key   = {C, minor()};

	constexpr static float paddingLines = 8;
	constexpr static float preferredNoteHeight = 8;

	Constraint measure(Canvas& canvas, Constraint impose) noexcept override;
	void       paint(Canvas& canvas) noexcept override;

	void drawNote(Canvas& canvas, float x, Note note) const noexcept;
	void drawNote(Canvas& canvas, float x, WholeNote note) const noexcept;

	float noteHeight() const noexcept { return std::floor(size().h / (staff.height() + paddingLines)); }
	float noteWidth()  const noexcept { return noteHeight() * 1.4f; }
	float lineOffset() const noexcept { return noteHeight() * (paddingLines / 2); }

	float line(Note note)   const noexcept { return line(key.wholeNote(note)); }
	float line(WholeNote n) const noexcept { return line(staff.lineof(n)); }
	float line(int line)    const noexcept { return (size().h - (line * noteHeight() + lineOffset())); }
private:
	void drawStaff(Canvas& canvas) const noexcept;
	void drawKey(Canvas& canvas) const noexcept;
};

} // namespace music::arranger
