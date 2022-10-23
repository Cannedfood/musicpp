#pragma once

#include "./Pitch.hpp"

#include "util/StaticVector.hpp"
#include <bitset>

namespace music {

struct IntervalSet {
    util::StaticVector<Interval, 16> relative_intervals;

    template<class Range = std::initializer_list<Interval>>
    constexpr static IntervalSet from_relative(Range&& range) noexcept {
        return IntervalSet { .relative_intervals = range };
    }
    template<class Range = std::initializer_list<Interval>>
    constexpr static IntervalSet from_absolute(Range&& range) noexcept {
        IntervalSet result;
        Interval last = Interval(0);
        for(auto& interval : range) {
            result.relative_intervals.push_back(Interval(interval - last));
            last = interval;
        }
        return result;
    }
    template<class Range = std::initializer_list<Pitch>>
    constexpr static IntervalSet from_pitches(Range&& range) noexcept {
        IntervalSet result;
        auto iter = std::begin(range);
        auto end = std::end(range);

        if(iter == end) return result;

        auto value = *iter;
        ++iter;
        while(iter != end) {
            auto previous_value = std::exchange(value, *iter);
            result.relative_intervals.push_back(Interval(value - previous_value));
            ++iter;
        }

        return result;
    }
    constexpr int num_inversions() const noexcept {
        return relative_intervals.size();
    }
    constexpr IntervalSet inversion(int n) const noexcept {
        auto pitches = apply_to_base({C, 0}).rotate(n);
        for(int i = 1; i < n; ++i) {
            while(pitches[i] < pitches[i - 1])
                pitches[i] += octave;
        }
        return from_pitches(pitches);
    }

    constexpr util::StaticVector<Pitch, 16> apply_to_base(Pitch base) const noexcept {
        util::StaticVector<Pitch, 16> result;
        result.push_back(base);
        for(size_t i = 0; i < relative_intervals.size(); i++) {
            base += relative_intervals[i];
            result.push_back(base);
        }
        return result;
    }
};

} // namespace music