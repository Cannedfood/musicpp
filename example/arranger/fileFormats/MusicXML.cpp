#include "MusicXML.hpp"

#include <stx/xml.hpp>
#include <stx/file2vector.hpp>

namespace music::arranger {

using namespace score;

Track ReadMusicXML(std::istream& istream) {
	std::string source = stx::stream2string(istream);

	stx::arena_allocator alloc;
	stx::xml::node n;
	n.parse_document(alloc, source.c_str());


}

} // namespace music::arranger
