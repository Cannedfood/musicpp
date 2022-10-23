#pragma once

#include "./Pitch.hpp"
#include "./IntervalSet.hpp"

namespace music::guitar {

constexpr inline std::array<Pitch, 6> standard_tuning(Pitch base = {E,2}) noexcept {
    return IntervalSet::from_relative({ perf4th, perf4th, perf4th, maj3rd, perf4th }).apply_to_base(base);
}

} // namespace music::guitar
