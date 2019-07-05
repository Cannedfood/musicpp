#pragma once

#include <wg/layout/Raster.hpp>
#include <musicpp.hpp>

namespace music::arranger {

class Sheet : public wg::Raster {
	class SheetBar;
public:
	void setSheet(music::sheet::Sheet& sheet) {
		Raster::clear();

		for(sheet::Track& track : sheet.tracks) {
			
		}
	}
private:
};

} // namespace music::arranger
