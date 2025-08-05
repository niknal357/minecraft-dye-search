#pragma once

#include <array>
#include <vector>
#include <mutex>

#include "reachabilityInstruction.h"
#include "color.h"
#include "minecraftColors.h"
#include "colorAccumulator.h"

class ReachabilityCalculator {
public:
	ReachabilityCalculator();

	bool calculate();

	unsigned int numReachableColors() const;

	bool writeResultsToFile(const std::string& filename) const;

private:
	std::vector<bool> updatedA;
	std::vector<bool> updatedB;
	std::vector<ReachabilityInstruction> reachability;

	bool searchColors(
		unsigned char depth,
		unsigned char highestColor,
		unsigned char dyeCost,
		const Color& startColor,
		std::vector<MCColor>& colors,
		ColorAccumulator& accumulatorh
	);

	bool searchColors(
		unsigned char depth,
		unsigned char highestColor,
		unsigned char dyeCost,
		const Color& startColor,
		std::vector<MCColor>& colors,
		ColorAccumulator& accumulator,
		std::array<std::mutex, 1024>* mutexPool
	);
};