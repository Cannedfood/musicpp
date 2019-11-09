#include "backend/jack/JackBackend.hpp"
#include "frontend/Frontend.hpp"

using namespace daw;

int main(int argc, char const* argv[]) {
	daw::backend::JackBackend backend;
	daw::frontend::Frontend   frontent(backend);

	frontent.Run();

	return 0;
}
