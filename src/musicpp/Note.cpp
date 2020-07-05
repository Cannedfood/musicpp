#include "Note.hpp"

#include <string>

namespace music {

std::string to_string(Note n) noexcept {
	if(n == Note::Null()) return "No note";
	return std::string(to_string(n.value)) + std::to_string(n.octave);
}

} // namespace music
