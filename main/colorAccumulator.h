#pragma once

#include "color.h"
#include "minecraftColors.h"

class ColorAccumulator {
public:
	ColorAccumulator();
	ColorAccumulator(Color initialColor);

	void addColor(Color color);
	void addColor(MCColor color) {
		addColor(toColor(color));
	}

	void removeColor(Color color);
	void removeColor(MCColor color) {
		removeColor(toColor(color));
	}

	Color getResultingColor() const;

private:
	unsigned int redTotal { 0 };
	unsigned int greenTotal { 0 };
	unsigned int blueTotal { 0 };
	unsigned int maxTotal { 0 };
	unsigned int contributors { 0 };
};