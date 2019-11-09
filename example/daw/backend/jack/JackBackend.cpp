#include "JackBackend.hpp"

namespace daw::backend {

class JackTrack : public Track {
	std::vector<jack::port> outputs;
	std::vector<jack::port> inputs;
public:
	// void Mute(bool b) {}
	// void Solo(bool b) {}
	// void EnableRecord(bool b) {}
};

JackBackend::JackBackend() {}

} // namespace daw::backend
