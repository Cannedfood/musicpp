#include "widgets/music/Measure.hpp"
#include "widgets/music/GuitarNeck.hpp"

using namespace music;
using namespace music::arranger;

stx::shared<Widget> toolButtons() {
	auto result = stx::make_shared<Raster>(4, 4);
	for(std::string s : {"A", "B", "C", "D", "E"})
		result->add(*stx::make_shared<Text>(s));
	return result;
}

struct PrintPositions : wg::Event {
	void onVisit(Widget& widget) const noexcept {
		printf("%s %p: %f %f %f %f\n", typeid(widget).name(), &widget, WG_XY(widget.offset()), WG_WH(widget.size()));
	}
};

int main(int argc, char const* argv[]) {
	wg::Application app;
	wg::Window window(app, "Hello world", 800, 600, Window::FitContent);

	app.useDefaultRessources();

	auto editorLayout = wg::Grid({
		{ 0,  20, 80 },
		{ 5,   1,  1 },
		{ 75,  2,  3 },
		{ 20,  4,  4 },
	});
	window.add(editorLayout);

	editorLayout.add(*toolButtons(), 2);

	wg::Raster list(4, 2);
	for(size_t i = 0; i < 5; i++) {
		list.add(*stx::make_shared<Measure>());
	}
	editorLayout.add(list, 3);

	editorLayout.add(*stx::make_shared<GuitarNeck>(), 4);

	app.run();

	return 0;
}
