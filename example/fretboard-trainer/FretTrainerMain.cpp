#include "../common-lib/widgets/music/GuitarNeck.hpp"
#include "wg/core/types/Constraint.hpp"

#include <cstdio>
#include <stx/random.hpp>


using namespace example::common;

class FretTrainer : public wg::Widget {
public:
	guitar::Tuning tuning = guitar::Standard;

	int  currentString;
	Note currentNote;

	FretTrainer() {
		nextNote();
	}

	GuitarNeckPainter neckPainter() const noexcept {
		return GuitarNeckPainter({30, 10}, {60, 20}, tuning, 0, 12);
	}

	Constraint measure(Canvas &canvas, const Constraint &impose) noexcept override {
		auto painter = neckPainter();
		auto sz = painter.size();
		printf("%g %g\n", sz.w, sz.h);
		return Constraint::exactly(sz).clamp(impose);
	}

	void paint(Canvas &canvas) noexcept override {
		auto painter = neckPainter();

		painter.paintNeck(canvas, White, White, LightGray);
		painter.paintNote(canvas, currentString, currentNote, "D#", White, Black, White);
	}

	void nextNote() {
		currentString = stx::rand(0, (int)tuning.size() - 1);
		currentNote   = tuning.at(currentString) + stx::rand(neckPainter().startFret, neckPainter().endFret);
	}
};

int main(int argc, char const* argv[]) {
	wg::Application app;

	wg::Window window(app, "Fret Trainer", 800, 600, Window::FitContent);

	app.useDefaultRessources();

	FretTrainer trainer;

	window.add(trainer);

	app.run();
}
