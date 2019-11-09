#pragma once

#include <musicpp/score/Track.hpp>

#include <iosfwd>

namespace music::fileformat {

score::Sheet ReadMusicXML(std::istream&);
score::Sheet ReadMusicXML(std::string const& path);

} // namespace music::fileformat
