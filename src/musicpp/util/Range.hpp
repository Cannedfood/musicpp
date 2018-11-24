#pragma once

#include <type_traits>

namespace music::util {

template<class Iter>
class Range {
public:
	using iterator = std::remove_reference_t<Iter>;

	iterator m_begin, m_end;

	constexpr Range(Iter begin, Iter end) noexcept :
		m_begin(begin),
		m_end(end)
	{}

	constexpr iterator const& begin() const noexcept { return m_begin; }
	constexpr iterator const& end()   const noexcept { return m_end; }
};

} // namespace music::util
