#pragma once

#include "Bitmap.h"

namespace kbb
{
	Bitmap convertEquirectangularMapToVerticalCross(const Bitmap& b);
	Bitmap convertVerticalCrossToCubeMapFaces(const Bitmap& b);
}
