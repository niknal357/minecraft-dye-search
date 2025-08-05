#include "colorAccumulator.h"

#include <cassert>
#include <algorithm>

ColorAccumulator::ColorAccumulator() {}

ColorAccumulator::ColorAccumulator(Color initialColor) {
	addColor(initialColor);
}

void ColorAccumulator::addColor(Color color) {
	redTotal += color.r;
	greenTotal += color.g;
	blueTotal += color.b;
	maxTotal += std::max({color.r, color.g, color.b});
	contributors++;
}

void ColorAccumulator::removeColor(Color color) {

	redTotal -= color.r;
	greenTotal -= color.g;
	blueTotal -= color.b;
	maxTotal -= std::max({color.r, color.g, color.b});
	contributors--;
}

Color ColorAccumulator::getResultingColor() const {
	assert(contributors > 0);

	float contributorsF = static_cast<float>(contributors);

	unsigned int redAverage = redTotal / contributorsF;
	unsigned int greenAverage = greenTotal / contributorsF;
	unsigned int blueAverage = blueTotal / contributorsF;
	float maxAverage = maxTotal / contributorsF;

	unsigned int brightness = std::max({ redAverage, greenAverage, blueAverage });
	float scale = maxAverage / static_cast<float>(brightness);

	return {
		static_cast<unsigned char>(redAverage * scale),
		static_cast<unsigned char>(greenAverage * scale),
		static_cast<unsigned char>(blueAverage * scale)
	};
}
