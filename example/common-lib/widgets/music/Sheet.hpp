#pragma once

#include <wg/layout/Raster.hpp>
#include <musicpp.hpp>

namespace example::common {

class SheetView : public wg::Raster {
public:

private:
	stx::shared<score::Sheet> sheet;
};

} // namespace example::common
