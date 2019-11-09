#pragma once

#include "../Backend.hpp"

#include "jack.hpp"

namespace daw::backend {

class JackBackend : public Backend {
public:
	JackBackend();
};

} // namespace daw::backend

