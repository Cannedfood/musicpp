#pragma once

#include <cstddef>
#include <stdexcept>
#include <memory>

namespace daw::frontend { class Frontend; }

namespace daw::backend {

class Plugin {
public:
	virtual void SetBufferSize(size_t bufferSize) {}
	virtual void SetSampleRate(size_t sampleRate) {}
	virtual void SetChannels  (size_t numChannels) {}
	virtual void Process      (float* const* buffers, size_t numSamples, size_t numChannels, size_t sampleRate) = 0;
	virtual ~Plugin() {}
};

class Track {
public:
	virtual void Mute(bool b) { throw std::runtime_error("Unimplemented"); }
	virtual void Solo(bool b) { throw std::runtime_error("Unimplemented"); }
	virtual void EnableRecord(bool b) { throw std::runtime_error("Unimplemented"); }
	virtual ~Track() {}
};

class Backend {
public:
	frontend::Frontend* Frontend = nullptr;

	virtual std::shared_ptr<Track> CreateTrack(int at) { return nullptr; }
};

} // namespace daw::backend
