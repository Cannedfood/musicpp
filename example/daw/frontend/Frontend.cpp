#include "Frontend.hpp"
#include "../backend/Backend.hpp"

#include <wg/core/Builder.hpp>

namespace daw::frontend {

class Track : public wg::Flexbox {
public:
	stx::shared<wg::Button> record, mute, solo;
	stx::shared<wg::Text>   title;

	Track(std::string name) noexcept {
		wg::Builder()
		.addDefaultBuilders()
		.parseChildren(
			wg::BindInfo {
				{ "record", record },
				{ "mute",   mute },
				{ "solo",   solo },
				{ "title",  title }
			},
			R"(
				<row>
					<raster cols="2" rows="2">
						<text   bind="title"/>
						<button bind="record"/>
						<button bind="mute"/>
						<button bind="solo"/>
					</raster>
					<text> Hello World! </text>
				</row>
			)",
			[this](auto w) { add(w); }
		);

		title->text(std::move(name));
	}
};

Frontend::Frontend(backend::Backend& backend) :
	backend(backend)
{
	backend.Frontend = this;

	wg::Builder()
	.addWindow(app)
	.addDefaultBuilders()
	.parse(
		wg::BindInfo {
			{ "MainWindow", window },
			{ "Tracks",     trackList }
		},
		R"(
			<window name="MainWindow" title="Hello" width="800" height="600">
				<list name="Tracks">
				</list>
			</window>
		)"
	);

	assert(window && trackList);

	app.useDefaultRessources();

	trackList->add(stx::make_shared<Track>("Guitar"));
	trackList->add(stx::make_shared<Track>("Bass"));
}
Frontend::~Frontend() noexcept {}

void Frontend::Run() { app.run(); }

} // namespace daw::frontend

