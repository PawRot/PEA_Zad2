
#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <vector>
#include <chrono>
#include <random>

class simulatedAnnealing {
private:
    std::vector<std::vector<int>> matrix;
    std::vector<int> path;
    int greedyCost;
    int numberOfCities;
    int stopCriterion;
    long double temperature; // current temperature
    long double lowestTemperature; // lowest temperature reached by the algorithm
    long double coolingRate;

    [[nodiscard]] int pathCost(const std::vector<int>& testedPath) const; // calculates the curent path cost

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> findShortestPath(); // runs the algorithm

    [[nodiscard]] long double calculateStartingTemperature() const; // calculates the starting temperature



public:
    std::vector<std::string> epochValuesAndTimes;

    simulatedAnnealing(const std::vector<std::vector<int>>& matrix, long double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult);

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> simulatedAnnealingAlgorithm();

    [[nodiscard]] long double getTemperature() const {
        return temperature;
    }
    [[nodiscard]] long double getLowestTemperature() const {
        return lowestTemperature;
    }

};



#endif //SIMULATEDANNEALING_H
