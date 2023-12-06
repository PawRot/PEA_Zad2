//
// Created by Michał Zychowicz on 03/12/2023.
//

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>

class simulatedAnnealing {
private:
    std::vector<std::vector<int>> matrix;
    std::vector<int> path;
    int greedyCost;
    int numberOfCities;
    int stopCriterion;
    long double temperature;
    long double coolingRate;

    [[nodiscard]] int pathCost(const std::vector<int>& testedPath) const;

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> findShortestPath();

    [[nodiscard]] double calculateStartingTemperature() const;


public:
    std::vector<std::string> epochValuesAndTimes;

    simulatedAnnealing(const std::vector<std::vector<int>>& matrix, long double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult);

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> simulatedAnnealingAlgorithm();

    [[nodiscard]] long double getTemperature() const {
        return temperature;
    }

};



#endif //SIMULATEDANNEALING_H
