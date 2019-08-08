#include "Measure.hpp"

namespace example::common {

Constraint Measure::measure(Canvas &canvas, Constraint const& impose) noexcept {
	float height = preferredNoteHeight * (staff.height() + paddingLines);
	return impose.with_min({ height, height }).with_pref({ height, height * 1.5f }).with_max({ Inf, Inf }).clamp(impose);
}

void Measure::paint(Canvas& canvas) noexcept {
	scale = area();

	drawStaff(canvas);
	drawKey(canvas);

	float x = 0;
	for(Note note : major().notes(C)) {
		drawNote(canvas, x+=noteHeight(), note);
	}
}

} // namespace example::common
