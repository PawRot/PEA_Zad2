//
// Created by Michał Zychowicz on 03/12/2023.
//

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

class simulatedAnnealing {
private:
    std::vector<std::vector<int>> matrix;
    std::vector<int> path;
    int greedyCost;
    int numberOfCities;
    int stopCriterion;
    long double temperature;
    double coolingRate;


public:
    simulatedAnnealing(const std::vector<std::vector<int>>& matrix, double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult);

    int pathCost(const std::vector<int>& path);

    std::vector<int> findShortestPath();

    std::tuple<int, std::vector<int>> simulatedAnnealingAlgorithm();

    [[nodiscard]] long double getTemperature() const {
        return temperature;
    }

};



#endif //SIMULATEDANNEALING_H
