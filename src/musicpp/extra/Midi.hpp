#pragma once

#include <variant>
#include "../score/Track.hpp"

// TODO

namespace music::extra {

struct MidiEvent {

};

template<class Callback> std::enable_if<std::is_invocable_v<Callback, MidiEvent>,
void> toMidi(score::Track const& track, Callback&& cb) {

}

} // namespace music::extra
