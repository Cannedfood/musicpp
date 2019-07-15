#pragma once

#include <wg/layout/Raster.hpp>
#include <musicpp.hpp>

namespace music::arranger {

class SheetView : public wg::Raster {
public:
	sheet::Sheet sheet;
private:
};

} // namespace music::arranger
