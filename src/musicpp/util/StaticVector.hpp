#pragma once

#include <cstdlib>
#include <array>
#include <algorithm>

namespace music::util {

template<class T, size_t Capacity>
class StaticVector : public std::array<T, Capacity> {
public:
	static_assert(std::is_trivially_destructible_v<T>, "T has to be trivially destructable");

	constexpr StaticVector() noexcept {}
	constexpr StaticVector(T defaultVal) noexcept {
		std::array<T, Capacity>::fill(defaultVal);
	}
	constexpr StaticVector(size_t count, T value) noexcept {
		for(size_t i = 0; i<count; i++) push_back(value);
	}
	constexpr StaticVector(std::initializer_list<T> args) noexcept {
		push_back(args.begin(), args.end());
	}

	constexpr T& push_back (T t) noexcept { return this->at(m_size++) = std::move(t); }
	template<class FwdIter>
	constexpr void push_back (FwdIter&& begin, FwdIter&& end) noexcept {
		for(auto iter = std::move(begin); iter != end; ++iter)
			push_back(*iter);
	}
	constexpr T& push_front(T t) noexcept {
		m_size++;
		for(int i = m_size - 1; i >= 0; i--) {
			this->at(i + 1) = this->at(i);
		}
		return this->at(0) = std::move(t);
	}
	void clear() noexcept { m_size = 0; }

	constexpr StaticVector rotate(int n) const noexcept {
		StaticVector copy = *this;
		if(n < 0) {
			n = size() - (-n % size());
		}
		n = n % size();
		std::rotate(copy.begin(), copy.begin() + n, copy.end());
		return copy;
	}

	template<class Pred = std::less<>>
	constexpr void sort_in_place(Pred less = {}) noexcept {
		// Insertion sort
		for(size_t i = 1; i < size(); i++) {
			T key = std::move(this->at(i));
			int j = i - 1;

			while (j >= 0 && less(key, this->at(j))) {
				this->at(j + 1) = std::move(this->at(j));
				j = j - 1;
			}
			this->at(j + 1) = std::move(key);
		}
	}

	constexpr T&       back()       noexcept { return at(m_size - 1); }
	constexpr T const& back() const noexcept { return at(m_size - 1); }

	constexpr void   resize(size_t n) noexcept { m_size = n; }
	constexpr size_t size()     const noexcept { return m_size; }
	constexpr bool   empty()    const noexcept { return m_size == 0; }

	constexpr bool contains(T const& t) const noexcept {
		for(auto& v : *this) { if(v == t) return true; }
		return false;
	}

	using iterator               = typename std::array<T, Capacity>::iterator;
	using const_iterator         = typename std::array<T, Capacity>::const_iterator;
	using reverse_iterator       = typename std::array<T, Capacity>::reverse_iterator;
	using const_reverse_iterator = typename std::array<T, Capacity>::const_reverse_iterator;

	constexpr iterator                 end()       noexcept { return this->begin()   + m_size; }
	constexpr const_iterator           end() const noexcept { return this->begin()   + m_size; }
	constexpr const_iterator          cend() const noexcept { return this->cbegin()  + m_size; }
	constexpr iterator                rend()       noexcept { return this->rbegin()  - m_size; }
	constexpr const_reverse_iterator crend() const noexcept { return this->crbegin() - m_size; }

	constexpr bool operator==(StaticVector const& other) const noexcept {
		if(size() != other.size()) return false;
		return std::equal(this->begin(), this->end(), other.begin(), other.end());
	}
	constexpr bool operator!=(StaticVector const& other) const noexcept { return !(*this == other); }

private:
	size_t m_size = 0;
};

template<class T, size_t Capacity>
StaticVector<T, Capacity> operator+(T a, StaticVector<T, Capacity> const& b) noexcept {
	auto result = b;
	result.push_front(std::move(a));
	return result;
}

template<class T, size_t Capacity>
StaticVector<T, Capacity> operator+(StaticVector<T, Capacity> const& a, T b) noexcept {
	auto result = a;
	result.push_back(std::move(b));
	return result;
}

} // namespace music::util
