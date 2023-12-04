//
// Created by Michał Zychowicz on 03/12/2023.
//

#include <iostream>
#include "simulatedAnnealing.h"



simulatedAnnealing::simulatedAnnealing(const std::vector<std::vector<int>>&matrix, long double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult)
: matrix(matrix), stopCriterion(stopCriterion), coolingRate(coolingRate) {
    numberOfCities = static_cast<int>(matrix.size());
    auto greedyPath = std::get<1>(greedyResult);
    path = {greedyPath.begin(), greedyPath.end() - 1};
    temperature = pow(numberOfCities, 2);
    greedyCost = std::get<0>(greedyResult);
}


int simulatedAnnealing::pathCost(const std::vector<int>&path) const {
    int cost = 0;
    for (int i = 0; i < numberOfCities - 1; i++) {
        cost += matrix[path[i]][path[i+1]];
    }
    cost += matrix[path[numberOfCities-1]][path[0]]; // Return to the starting city
    return cost;
}

std::vector<int> simulatedAnnealing::findShortestPath() {
    int currentCost = greedyCost;

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> bestPath = {INT_MAX, {}, std::chrono::duration<float>::zero()};

    std::chrono::duration<float> timeElapsed = std::chrono::duration<float>::zero();
    const auto start = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) < std::chrono::seconds(stopCriterion)) {
        std::vector<int> newPath = path;

        for (int i = 0; i <= (numberOfCities*3)/2; i++) {
            int swapIndex1 = rand() % numberOfCities;
            int swapIndex2;
            do {
                swapIndex2 = rand() % numberOfCities;
            } while (swapIndex1 == swapIndex2);
            std::swap(newPath[swapIndex1], newPath[swapIndex2]);

            int newCost = pathCost(newPath);
            if (newCost < currentCost) {
                path = newPath;
                currentCost = newCost;

                if (newCost < std::get<0>(bestPath)) {
                    bestPath = {newCost, newPath, std::chrono::steady_clock::now() - start};
                }

            } else if (exp((currentCost - newCost) / temperature) > ((double) rand() / (RAND_MAX))) {
                path = newPath;
                currentCost = newCost;
            }
        }
        // std::cout << temperature << std::endl;
        temperature *= coolingRate;
        timeElapsed = std::chrono::steady_clock::now() - start;
    }

    // std::cout << std::endl << std::get<0>(bestPath) << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(bestPath)).count() << std::endl;
    return path;
}

std::tuple<int, std::vector<int>> simulatedAnnealing::simulatedAnnealingAlgorithm() {
    auto result = findShortestPath();
    auto cost = pathCost(result);
    result.push_back(result[0]);
    return {cost, result};
}






