#include <iostream>
#include <array>
#include <string>

#include "color.h"
#include "minecraftColors.h"
#include "colorAccumulator.h"
#include "reachabilityInstruction.h"
#include "reachabilityCalculator.h"

int main() {
	ReachabilityCalculator calculator = ReachabilityCalculator();
	std::cout << "There are " << calculator.numReachableColors() << " reachable colors." << std::endl;
	calculator.writeResultsToFile("initial_reachability_results.txt");

	int i = 0;
	while(calculator.calculate()){
		std::cout << "There are " << calculator.numReachableColors() << " reachable colors after calculation." << std::endl;
		calculator.writeResultsToFile("reachability_results_" + std::to_string(i++) + ".txt");
	}

	const std::string finalOutputFile = "final_reachability_results.txt";
	std::cout << "Total reachable colors: " << calculator.numReachableColors() << std::endl;
	calculator.writeResultsToFile(finalOutputFile);

	return 0;
}
