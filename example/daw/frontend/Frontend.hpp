#pragma once

#include <wg.hpp>

namespace daw::backend { class Backend; }

namespace daw::frontend {

class Frontend {
public:
	Frontend(backend::Backend& backend);
	~Frontend() noexcept;

	void Run();

private:
	wg::Application app;
	stx::shared<wg::Window>  window;
	stx::shared<wg::Flexbox> trackList;
	backend::Backend& backend;
};

} // namespace daw::frontend
