#pragma once
#include <string>
#include <cstdio>
#include <cstdint>
 
struct Color
{
	unsigned char r{};
	unsigned char g{};
	unsigned char b{};

	constexpr Color() = default;

	constexpr explicit Color(uint32_t packed) :
		r(static_cast<unsigned char>((packed >> 16) & 0xFF)),
		g(static_cast<unsigned char>((packed >> 8) & 0xFF)),
		b(static_cast<unsigned char>(packed & 0xFF)) {}

	constexpr Color(
		unsigned char red,
		unsigned char green,
		unsigned char blue
	) : r(red), g(green), b(blue) {}

	constexpr bool operator==(const Color& o) const {
		return r == o.r && g == o.g && b == o.b;
	}

	constexpr bool operator!=(const Color& o) const {
		return !(*this == o);
	}

	constexpr uint32_t toUint() const {
		return (static_cast<uint32_t>(r) << 16) |
			   (static_cast<uint32_t>(g) <<  8) |
			   static_cast<uint32_t>(b);
	}

	constexpr bool isValid() const {
		return *this != INVALID;
	}

	std::string toString() const {
		return "rgb("
			+ std::to_string(r) + ", "
			+ std::to_string(g) + ", "
			+ std::to_string(b) + ")";
	}

	std::string toHex() const {
		char buf[8];
		std::snprintf(buf, sizeof buf, "#%02X%02X%02X", r, g, b);
		return buf;
	}

	static const Color INVALID;
};

inline constexpr Color Color::INVALID{0, 0, 0};
