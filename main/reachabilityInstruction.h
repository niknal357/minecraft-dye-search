#pragma once

#include <array>

#include "color.h"
#include "minecraftColors.h"

struct ReachabilityInstruction {
	unsigned char cost { 255 };
	Color startColor { Color::INVALID };
	std::array<MCColor, 8> colors { MCColor::INVALID };
};