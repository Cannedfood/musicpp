#include <wg.hpp>

#include "jack.hpp"

class Buffer {
public:
	Buffer(size_t reserve = 44100 * 20) { mBuffer.reserve(reserve); }

	void append(unsigned nsamples, float* data) {
		mBuffer.insert(mBuffer.end(), data, data + nsamples);
	}
	void clear() {
		mBuffer.clear();
	}

	unsigned read(unsigned at, unsigned nsamples, float* data) {
		if(mBuffer.empty()) {
			std::fill_n(data, nsamples, 0);
			return 0;
		}

		unsigned n = std::min<unsigned>(nsamples, mBuffer.size() - at);
		std::copy_n(mBuffer.data() + at, n, data);

		unsigned remainder = nsamples - n;
		std::copy_n(mBuffer.begin(), remainder, data + n);

		return (at + nsamples) % mBuffer.size();
	}

	unsigned write(unsigned at, unsigned const nsamples, float* data) {
		if(mBuffer.empty()) { return 0; }

		unsigned const n = std::min<unsigned>(nsamples, mBuffer.size() - at);
		for(size_t i = 0; i < n; i++) {
			mBuffer[at + n] += data[i];
		}

		// unsigned const remainder = nsamples - n;
		// for(size_t i = 0; i < remainder; i++) {
		// 	mBuffer[i] += data[i + n];
		// }

		return (at + nsamples) % mBuffer.size();
	}

	size_t size() { return mBuffer.size(); }

private:
	std::vector<float> mBuffer;
};

class MultiChannelBuffer {
public:
	MultiChannelBuffer(size_t channels) :
		mBuffers(channels)
	{}

	void append(unsigned nsamples, std::initializer_list<float*> data) {
		assert(data.size() == mBuffers.size());

		unsigned b = 0;
		for(float* d : data) {
			mBuffers[b].append(nsamples, d);
			b++;
		}
	}

	unsigned read  (unsigned at, unsigned nsamples, std::initializer_list<float*> data) {
		assert(data.size() == mBuffers.size());

		unsigned result = 0;

		unsigned b = 0;
		for(float* d : data) {
			result = mBuffers[b].read(at, nsamples, d);
			b++;
		}

		return result;
	}

	unsigned write (unsigned at, unsigned nsamples, std::initializer_list<float*> data) {
		assert(data.size() == mBuffers.size());

		unsigned result = 0;

		unsigned b = 0;
		for(float* d : data) {
			result = mBuffers[b].write(at, nsamples, d);
			b++;
		}

		return result;
	}

	void clear() {
		for(auto& b : mBuffers) b.clear();
	}
private:
	std::vector<Buffer> mBuffers;
};

class Track {
public:
	MultiChannelBuffer mBuffer;

	
};

class Recorder : public wg::Flexbox {
public:
	Recorder() :
		m_buffer(2)
	{
		m_client = jack::client("recorder", JackNoStartServer);

		// Inputs
		m_left_input   = jack::port(m_client, "left/mono in",  JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
		m_right_input  = jack::port(m_client, "right in",      JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);

		// Outputs
		m_left_output  = jack::port(m_client, "left/mono out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
		m_right_output = jack::port(m_client, "right out",     JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);

		jack_set_process_callback(m_client, [](unsigned samples, void* arg) { return ((Recorder*) arg)->process(samples); }, this);

		wrap(false);
		horizontal(false);
		add(m_state_text);

		m_client.activate();

		m_left_input.connect_from("out_0",  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
		m_right_input.connect_from("out_1",  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
		m_left_output.connect_to ("playback_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
		m_right_output.connect_to ("playback_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
	}

	int process(unsigned samples) {
		if(m_state == 0) {
			std::fill_n(m_left_output.buffer(samples), samples, 0);
			std::fill_n(m_right_output.buffer(samples), samples, 0);
		}
		if(m_state == 1) {
			m_buffer.append(samples, {
				m_left_input.buffer(samples),
				m_right_input.buffer(samples)
			});
		}
		if(m_state == 2 || m_state == 3) {
			unsigned oldPos = m_read_position;
			m_read_position = m_buffer.read(m_read_position, samples, {
				m_left_output.buffer(samples),
				m_right_output.buffer(samples)
			});

			if(m_state == 3) {
				m_buffer.write(oldPos, samples, {
					m_left_input.buffer(samples),
					m_right_input.buffer(samples)
				});
			}
		}
		return 0;
	}

	void on(wg::KeyEvent const& e) noexcept override {
		if(e.state == wg::Pressed && e.keycode == ' ') {
			printf("Keycode: %i\n", e.scancode);
			printf("From %i\n", m_state);
			setState(m_state + 1);
		}
	}

	void setState(int state) {
		printf("%i\n", state);
		m_state = state;
		switch(m_state) {
		case 0: m_state_text.text("Standby"); break;
		case 1: m_state_text.text("Recording"); m_buffer.clear(); break;
		case 2: m_state_text.text("Playing"); break;
		// case 3: m_state_text.text("Overdub"); break;
		}
	}

private:
	int m_state = 0;
	wg::Text m_state_text;

	MultiChannelBuffer m_buffer;
	unsigned           m_read_position = 0;

	jack::client m_client;
	jack::port   m_left_input;
	jack::port   m_right_input;
	jack::port   m_left_output;
	jack::port   m_right_output;
};

int main(int argc, char const* argv[]) {
	wg::Application app;

	wg::Window window(app, "Hello", 800, 600);

	Recorder recorder;

	window.add(recorder);

	app.useDefaultRessources();
	app.run();
}
