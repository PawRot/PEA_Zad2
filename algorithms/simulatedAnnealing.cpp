//
// Created by Michał Zychowicz on 03/12/2023.
//

#include "simulatedAnnealing.h"


simulatedAnnealing::simulatedAnnealing(const std::vector<std::vector<int>>&matrix, double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult)
: matrix(matrix), stopCriterion(stopCriterion), coolingRate(coolingRate) {
    numberOfCities = static_cast<int>(matrix.size());
    auto greedyPath = std::get<1>(greedyResult);
    path = {greedyPath.begin(), greedyPath.end() - 1};
    temperature = numberOfCities * numberOfCities;
    greedyCost = std::get<0>(greedyResult);
}


int simulatedAnnealing::pathCost(const std::vector<int>&path) {
    int cost = 0;
    for (int i = 0; i < numberOfCities - 1; i++) {
        cost += matrix[path[i]][path[i+1]];
    }
    cost += matrix[path[numberOfCities-1]][path[0]]; // Return to the starting city
    return cost;
}

std::vector<int> simulatedAnnealing::findShortestPath() {
    int currentCost = greedyCost;
    std::chrono::duration<float> timeElapsed = std::chrono::duration<float>::zero();
    // std::chrono::d
    const auto start = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) < std::chrono::seconds(stopCriterion)) {
        std::vector<int> newPath = path;
        int swapIndex1 = rand() % numberOfCities;
        int swapIndex2 = rand() % numberOfCities;
        std::swap(newPath[swapIndex1], newPath[swapIndex2]);

        int newCost = pathCost(newPath);
        if (newCost < currentCost || exp((currentCost - newCost) / temperature) > ((double) rand() / (RAND_MAX))) {
            path = newPath;
            currentCost = newCost;
        }

        temperature *= coolingRate;
        timeElapsed = std::chrono::steady_clock::now() - start;
    }

    return path;
}

std::tuple<int, std::vector<int>> simulatedAnnealing::simulatedAnnealingAlgorithm() {
    auto result = findShortestPath();
    auto cost = pathCost(result);
    result.push_back(result[0]);
    return {cost, result};
}






