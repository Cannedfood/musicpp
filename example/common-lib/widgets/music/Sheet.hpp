#pragma once

#include <wg/layout/Raster.hpp>
#include <musicpp.hpp>

namespace example::common {

class SheetView : public wg::Raster {
public:
	sheet::Sheet sheet;
private:
};

} // namespace example::common
