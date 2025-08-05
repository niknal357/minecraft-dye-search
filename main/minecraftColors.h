#pragma once

#include "color.h"

enum MCColor {
	WHITE = 0,
	ORANGE = 1,
	MAGENTA = 2,
	LIGHT_BLUE = 3,
	YELLOW = 4,
	LIME = 5,
	PINK = 6,
	GRAY = 7,
	LIGHT_GRAY = 8,
	CYAN = 9,
	PURPLE = 10,
	BLUE = 11,
	BROWN = 12,
	GREEN = 13,
	RED = 14,
	BLACK = 15,
	INVALID = 16
};

constexpr Color toColor(MCColor mcColor) {
	switch (mcColor) {
		case WHITE:			return Color{249, 255, 254};
		case ORANGE:		return Color{249, 128,  29};
		case MAGENTA:		return Color{199,  78, 189};
		case LIGHT_BLUE:	return Color{ 58, 179, 218};
		case YELLOW:		return Color{254, 216,  61};
		case LIME:			return Color{128, 199,  31};
		case PINK:			return Color{243, 139, 170};
		case GRAY:			return Color{ 71,  79,  82};
		case LIGHT_GRAY:	return Color{157, 157, 151};
		case CYAN:			return Color{ 22, 156, 156};
		case PURPLE:		return Color{137,  50, 184};
		case BLUE:			return Color{ 60,  68, 170};
		case BROWN:			return Color{131,  84,  50};
		case GREEN:			return Color{ 94, 124,  22};
		case RED:			return Color{176,  46,  38};
		case BLACK:			return Color{ 29,  29,  33};
		default:			return Color::INVALID;
	}
}

inline std::string toString(MCColor mcColor) {
	switch (mcColor) {
		case WHITE:			return "White";
		case ORANGE:		return "Orange";
		case MAGENTA:		return "Magenta";
		case LIGHT_BLUE:	return "Light Blue";
		case YELLOW:		return "Yellow";
		case LIME:			return "Lime";
		case PINK:			return "Pink";
		case GRAY:			return "Gray";
		case LIGHT_GRAY:	return "Light Gray";
		case CYAN:			return "Cyan";
		case PURPLE:		return "Purple";
		case BLUE:			return "Blue";
		case BROWN:			return "Brown";
		case GREEN:			return "Green";
		case RED:			return "Red";
		case BLACK:			return "Black";
		default:			return "Invalid Color";
	}
}