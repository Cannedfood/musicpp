#pragma once

#include <jack/jack.h>

#include <memory>
#include <utility>

namespace jack {

class result {
public:
	using value_t = std::underlying_type_t<jack_status_t>;

	result(value_t value = 0) noexcept : m_value(value) {}
	result(result&& other) noexcept : m_value(std::exchange(other.m_value, 0)) {}
	result(result const&) = delete;
	~result() { reset(); }

	void reset() {
		if(m_value == 0) return;

		m_value = 0;
		throw std::runtime_error("There was a jack error");
	}

	operator jack_status_t*() {
		reset();
		return reinterpret_cast<jack_status_t*>(&m_value);
	}
private:
	value_t m_value;
};

struct client_view {
	jack_client_t* m_client = nullptr;

	client_view(std::nullptr_t = nullptr) noexcept {}

	result activate()   { return jack_activate(m_client); }
	result deactivate() { return jack_deactivate(m_client); }

	operator jack_client_t*() noexcept { return m_client; }
};

struct client : client_view {
	client(const char* name, jack_options_t options) {
		result res;
		m_client = jack_client_open(name, options, res);
	}
	~client() noexcept { reset(); }

	client(std::nullptr_t = nullptr) noexcept {}
	client(client const&) = delete;
	client(client&& c) noexcept : client() { *this = std::move(c); }
	client& operator=(client&& other) {
		reset();
		m_client = std::exchange(other.m_client, nullptr);
		return *this;
	}

	void reset() {
		if(m_client) {
			jack_client_close(m_client);
			m_client = nullptr;
		}
	}
};

struct port_view {
	jack_port_t* m_port = nullptr;
	client_view  m_client;

	const char* name() noexcept { return jack_port_name(m_port); }

	void connect_internal(const char* port_name_pattern, const char* port_type_pattern, int flags, bool swap) noexcept {
		const char* name = this->name();

		const char** targetPorts = jack_get_ports(m_client, port_name_pattern, port_type_pattern, flags);
		for(const char** portName = targetPorts; *portName != nullptr; portName++) {
			const char* from = name;
			const char* to   = *portName;
			if(swap) std::swap(from, to);
			printf("[jack]: Connect %s -> %s\n", from, to);
			jack_connect(m_client, from, to);
		}
		free(targetPorts);
	}

	void connect_to  (const char* port_name_pattern, const char* port_type_pattern, int flags) noexcept { connect_internal(port_name_pattern, port_type_pattern, flags, false); }
	void connect_from(const char* port_name_pattern, const char* port_type_pattern, int flags) noexcept { connect_internal(port_name_pattern, port_type_pattern, flags, true); }

	float*       buffer(unsigned nsamples)       { return (float*)jack_port_get_buffer(m_port, nsamples); }
	float const* buffer(unsigned nsamples) const { return (float*)jack_port_get_buffer(m_port, nsamples); }

	operator jack_port_t*() noexcept { return m_port; }
};

struct port : port_view {
	port(client_view client, const char* name, const char* type, unsigned long flags, unsigned long buffer_size = 0) noexcept {
		this->m_port = jack_port_register(m_client = client, name, type, flags, buffer_size);
	}
	~port() noexcept {
		reset();
	}

	port(std::nullptr_t = nullptr) noexcept {}
	port(port const& other) = delete;
	port(port&& other) noexcept : port_view { std::exchange(other.m_port, nullptr), std::exchange(other.m_client, nullptr) } {}
	port& operator=(port&& other) noexcept {
		reset();
		m_port   = std::exchange(other.m_port, nullptr);
		m_client = std::exchange(other.m_client, nullptr);
		return *this;
	}

	void reset() noexcept {
		if(m_client) {
			jack_port_unregister(m_client, this->m_port);
			m_client = nullptr;
		}
	}
};

} // namespace jack
