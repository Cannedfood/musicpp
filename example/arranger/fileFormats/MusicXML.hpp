#pragma once

#include <musicpp/score/Track.hpp>

namespace music::arranger {

score::Track ReadMusicXML(std::istream&);

} // namespace music::arranger
