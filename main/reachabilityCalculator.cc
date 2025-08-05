#include "reachabilityCalculator.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

ReachabilityCalculator::ReachabilityCalculator() :
	updatedA(1u << 24, false),
	updatedB(1u << 24, false),
	reachability(1u << 24, ReachabilityInstruction {}) {
	std::vector<MCColor> initialColors;
	initialColors.reserve(8);
    ColorAccumulator acc;
	searchColors(7, 15, 0, Color::INVALID, initialColors, acc, nullptr);
	std::swap(updatedA, updatedB);
}

bool ReachabilityCalculator::calculate() {
	unsigned int totalToProcess = numReachableColors();
	std::atomic<unsigned int> processedCount(0);
	std::atomic<bool> updatedSomething(false);

	constexpr size_t MUTEX_POOL_SIZE = 1024;
	std::array<std::mutex, MUTEX_POOL_SIZE> mutexPool;

	std::fill(updatedB.begin(), updatedB.end(), false);

	std::vector<unsigned int> indicesToProcess;
	for (unsigned int i = 0; i < reachability.size(); ++i) {
		if (updatedA[i]) {
			indicesToProcess.push_back(i);
		}
	}

	const unsigned int numThreads = std::min(
		std::max(1u, std::thread::hardware_concurrency()),
		static_cast<unsigned int>(indicesToProcess.size())
	);

	std::vector<std::thread> threads;
	const unsigned int chunkSize = (indicesToProcess.size() + numThreads - 1) / numThreads;

	for (unsigned int threadId = 0; threadId < numThreads; ++threadId) {
		threads.emplace_back([&, threadId]() {
			const unsigned int start = threadId * chunkSize;
			const unsigned int end = std::min(start + chunkSize, static_cast<unsigned int>(indicesToProcess.size()));

			for (unsigned int idx = start; idx < end; ++idx) {
				const unsigned int i = indicesToProcess[idx];

				unsigned int currentProcessed = processedCount.fetch_add(1) + 1;
				if (threadId == 0){
					std::cout << "Processing color " << i << " ("
						<< currentProcessed << "/" << totalToProcess << ")" << std::endl;
				}

				std::vector<MCColor> colors;
				colors.reserve(8);

                ColorAccumulator acc = {Color{i}};
				bool localUpdated = searchColors(
					7,
					15,
					reachability[i].cost,
					Color{i},
					colors,
					acc,
					&mutexPool
				);

				if (localUpdated) {
					updatedSomething.store(true);
				}
			}
		});
	}

	for (auto& thread : threads) {
		thread.join();
	}

	std::swap(updatedA, updatedB);
	return updatedSomething.load();
}

bool ReachabilityCalculator::searchColors(
	unsigned char depth,
	unsigned char highestColor,
	unsigned char dyeCost,
	const Color& startColor,
	std::vector<MCColor>& colors,
	ColorAccumulator& accumulator,
	std::array<std::mutex, 1024>* mutexPool
) {
	bool updatedSomething = false;
	const unsigned char thisDyeCost = dyeCost + 1;

	if (colors.capacity() < colors.size() + depth + 1) {
		colors.reserve(colors.size() + depth + 1);
	}

	for (unsigned char i = 0; i <= highestColor; i++) {
		const MCColor color = static_cast<MCColor>(i);
		colors.push_back(color);
		accumulator.addColor(color);

		const Color resultingColor = accumulator.getResultingColor();
		const unsigned int resultingColorIndex = resultingColor.toUint();

		if (mutexPool) {
			const size_t mutexIndex = resultingColorIndex % mutexPool->size();
			std::lock_guard<std::mutex> lock((*mutexPool)[mutexIndex]);

			ReachabilityInstruction& instruction = reachability[resultingColorIndex];
			if (instruction.cost > thisDyeCost) {
				instruction.cost = thisDyeCost;
				instruction.startColor = startColor;

				const size_t colorsSize = colors.size();
				const size_t copySize = std::min(colorsSize, static_cast<size_t>(8));

				for (size_t j = 0; j < copySize; ++j) {
					instruction.colors[j] = colors[j];
				}
				for (size_t j = copySize; j < 8; ++j) {
					instruction.colors[j] = MCColor::INVALID;
				}

				updatedB[resultingColorIndex] = true;
				updatedSomething = true;
			}
		} else {
			ReachabilityInstruction& instruction = reachability[resultingColorIndex];
			if (instruction.cost > thisDyeCost) {
				instruction.cost = thisDyeCost;
				instruction.startColor = startColor;

				const size_t colorsSize = colors.size();
				const size_t copySize = std::min(colorsSize, static_cast<size_t>(8));

				for (size_t j = 0; j < copySize; ++j) {
					instruction.colors[j] = colors[j];
				}
				for (size_t j = copySize; j < 8; ++j) {
					instruction.colors[j] = MCColor::INVALID;
				}

				updatedB[resultingColorIndex] = true;
				updatedSomething = true;
			}
		}

		if (depth != 0 && thisDyeCost < std::numeric_limits<unsigned char>::max() - 1) {
			updatedSomething = searchColors(
				depth - 1,
				i,
				thisDyeCost,
				startColor,
				colors,
				accumulator,
				mutexPool
			) || updatedSomething;
		}

		accumulator.removeColor(color);
		colors.pop_back();
	}
	return updatedSomething;
}


unsigned int ReachabilityCalculator::numReachableColors() const {
	unsigned int count = 0;
	for (const auto& instruction : reachability) {
		if (instruction.cost != std::numeric_limits<unsigned char>::max()) {
			count++;
		}
	}
	return count;
}

bool ReachabilityCalculator::writeResultsToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
		return false;
	}

	file << "Reachability Results\n";
	file << "===================\n\n";
	file << "Total reachable colors: " << numReachableColors() << "\n\n";

	unsigned int count = 0;
	for (unsigned int i = 0; i < reachability.size(); ++i) {
		const ReachabilityInstruction& instruction = reachability[i];

		// Skip colors that are not reachable
		if (instruction.cost == std::numeric_limits<unsigned char>::max()) {
			continue;
		}

		count++;
		Color resultColor(i);

		file << "#" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(resultColor.r)
		     << std::setw(2) << static_cast<int>(resultColor.g) << std::setw(2) << static_cast<int>(resultColor.b)
		     << std::dec << " | ";

		bool firstColor = true;
		for (const MCColor& color : instruction.colors) {
			if (color == MCColor::INVALID) {
				break;
			}
			if (!firstColor) {
				file << "+";
			}
			file << toString(color);
			firstColor = false;
		}

		file << " | ";

		// Start color
		if (instruction.startColor != Color::INVALID) {
			file << "#" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(instruction.startColor.r)
			     << std::setw(2) << static_cast<int>(instruction.startColor.g) << std::setw(2) << static_cast<int>(instruction.startColor.b) << std::dec;
		} else {
			file << "BASE";
		}

		file << "\n";
	}

	file.close();
	std::cout << "Results written to " << filename << std::endl;
	return true;
}
