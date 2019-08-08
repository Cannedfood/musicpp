#pragma once

#include <musicpp/score/Track.hpp>

namespace example::common {

using namespace music;

score::Track ReadMusicXML(std::istream&);

} // namespace example::common
