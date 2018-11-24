#pragma once

#include "Enums.hpp"
#include "Note.hpp"

#include <tuple>
#include <cassert>

namespace music {

struct WholeNote {
	WholeNoteValue value;
	int            octave;

	constexpr inline
	WholeNote(WholeNoteValue val = NOTE_C, int octave = 4) noexcept :
		value(val),
		octave(octave)
	{}

	constexpr inline
	WholeNote(NoteValue val, int octave = 4) noexcept :
		value(whole(val)),
		octave(octave)
	{
		assert(is_whole(val)); // Requires c++14 supposedly (because constexpr)
	}

	constexpr explicit inline
	WholeNote(Note n) : WholeNote(n.value, n.octave) {}

	constexpr explicit inline WholeNote(int number) noexcept;
	constexpr explicit inline operator int() const noexcept;
	constexpr          inline operator Note() const noexcept;

	friend constexpr inline WholeNote& operator+=(WholeNote& a, int interval) noexcept;
	friend constexpr inline WholeNote& operator-=(WholeNote& a, int interval) noexcept;

	constexpr auto all_values() const noexcept { return std::tie(octave, value); }

	constexpr bool operator==(WholeNote const& other) const noexcept { return all_values() == other.all_values(); }
	constexpr bool operator!=(WholeNote const& other) const noexcept { return all_values() != other.all_values(); }
	constexpr bool operator> (WholeNote const& other) const noexcept { return all_values() >  other.all_values(); }
	constexpr bool operator>=(WholeNote const& other) const noexcept { return all_values() >= other.all_values(); }
	constexpr bool operator< (WholeNote const& other) const noexcept { return all_values() <  other.all_values(); }
	constexpr bool operator<=(WholeNote const& other) const noexcept { return all_values() <= other.all_values(); }

	constexpr static WholeNote Null() noexcept { return WholeNote(NOTE_C, std::numeric_limits<int>::max()); }

	constexpr static WholeNote Floor(Note n) noexcept { return WholeNote(whole(n.value, SHARP), n.octave); }
	constexpr static WholeNote Ceil (Note n) noexcept { return WholeNote(whole(n.value, FLAT),  n.octave); }
};

// ** Mathematical operators *******************************************************
constexpr inline WholeNote operator+(WholeNote n, int interval) noexcept;
constexpr inline WholeNote operator-(WholeNote n, int interval) noexcept;
constexpr inline int operator-(WholeNote a, WholeNote b) noexcept;


// =============================================================
// == Inline implementation =============================================
// =============================================================

// ** Midi conversion *******************************************************

constexpr inline WholeNote::operator int() const noexcept {
	return (octave + 1) * 7 + value;
}
constexpr inline WholeNote::operator Note() const noexcept {
	return Note(chromatic(value), octave);
}
constexpr inline WholeNote::WholeNote(int i) noexcept :
	WholeNote((WholeNoteValue) (i % 7), (int)(i / 7u) - 1 )
{}

// ** Mathematical operators *******************************************************

constexpr inline WholeNote  operator+ (WholeNote  n, int interval) noexcept { return WholeNote(int(n) + interval); }
constexpr inline WholeNote  operator- (WholeNote  n, int interval) noexcept { return WholeNote(int(n) - interval); }
constexpr inline WholeNote& operator+=(WholeNote& n, int interval) noexcept { return n = (n + interval); }
constexpr inline WholeNote& operator-=(WholeNote& n, int interval) noexcept { return n = (n - interval); }
constexpr inline int   operator- (WholeNote a, WholeNote b)        noexcept { return int(a) - int(b); }

} // namespace music
