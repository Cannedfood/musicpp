#pragma once

#include "../Note.hpp"
#include "../util/StaticVector.hpp"

#include <array>

namespace music::guitar {

class Tuning : public util::StaticVector<Note, 12> {
public:
	using StaticVector::StaticVector;

	Tuning(StaticVector const& v) noexcept :
		StaticVector(v)
	{}

	constexpr Tuning with_extra_lowest(Note n)  const noexcept { Tuning t = *this; t.push_front(n); return t; }
	constexpr Tuning with_extra_highest(Note n) const noexcept { Tuning t = *this; t.push_front(n); return t; }
	constexpr Tuning with_lowest(Note n)        const noexcept { Tuning t = *this; t.at(0) = n; return t; }

	constexpr size_t numStrings() const noexcept { return size(); }
};

static Tuning Standard  = { {E,2}, {A,2}, {D,3}, {G,3}, {B,3}, {E,4} };
static Tuning DropC     = Standard.with_lowest({C,2});

static Tuning Standard7      = Standard.with_extra_lowest({B,1});
static Tuning Standard7DropA = Standard.with_extra_lowest({A,1});

} // namespace music::guitar
