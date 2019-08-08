#pragma once

#include <musicpp.hpp>
#include <wg.hpp>

#include "BarPainter.hpp"

namespace example::common {

using namespace wg;

class Measure : public Widget, public BarPainter {
public:
	constexpr static float preferredNoteHeight = 8;

	Constraint measure(Canvas& canvas, Constraint const& impose) noexcept override;
	void       paint(Canvas& canvas) noexcept override;
};

} // namespace example::common