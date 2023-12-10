
#include <iostream>
#include "simulatedAnnealing.h"



simulatedAnnealing::simulatedAnnealing(const std::vector<std::vector<int>>&matrix, const long double coolingRate, const int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult)
: matrix(matrix), stopCriterion(stopCriterion), coolingRate(coolingRate) {
    numberOfCities = static_cast<int>(matrix.size());
    auto greedyPath = std::get<1>(greedyResult);
    path = {greedyPath.begin(), greedyPath.end() - 1};
    temperature = calculateStartingTemperature();
    lowestTemperature = temperature;
    greedyCost = std::get<0>(greedyResult);
    epochValuesAndTimes = {};
}


long double simulatedAnnealing::calculateStartingTemperature() const {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution index(0, numberOfCities - 1);


    int numberOfSwaps = numberOfCities * (numberOfCities-1) * 4;
    numberOfSwaps = numberOfSwaps > 15000 ? 15000 : numberOfSwaps; // 15000 is the maximum number of swaps
    int sum = 0;

    for (int i = 0; i < numberOfSwaps; i++) {
        auto neighborPath = path;
        const int swapIndex1 = index(mt);
        int swapIndex2;
        do {
            swapIndex2 = index(mt);
        } while (swapIndex1 == swapIndex2);
        std::swap(neighborPath[swapIndex1], neighborPath[swapIndex2]);
        sum += abs(pathCost(path) - pathCost(neighborPath)); // abs() because the difference can be negative
    }

    sum /= numberOfSwaps; // Average difference
    return (-1*sum)/log(0.99); // 0.99 is the probability of accepting a worse solution
}


int simulatedAnnealing::pathCost(const std::vector<int>&testedPath) const {
    int cost = 0;
    for (int i = 0; i < numberOfCities - 1; i++) {
        cost += matrix[testedPath[i]][testedPath[i + 1]];
    }
    cost += matrix[testedPath[numberOfCities - 1]][testedPath[0]]; // Return to the starting city
    return cost;
}

std::tuple<int, std::vector<int>, std::chrono::duration<float>> simulatedAnnealing::findShortestPath() {
    int currentCost = greedyCost;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution index(0, numberOfCities - 1); // index for swapping cities is from 0 to numberOfCities - 1
    std::uniform_real_distribution<double> dist(0.0, 1.0); // random double from 0 to 1 for probability of accepting a worse solution

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> bestSolution = {INT_MAX, {}, std::chrono::duration<float>::zero()};

    std::chrono::duration<float> timeElapsed = std::chrono::duration<float>::zero();
    const auto start = std::chrono::steady_clock::now(); // start time
    bool pathChanged = false; // if the path has changed in the last iteration
    while (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) < std::chrono::seconds(stopCriterion)) {

        if (temperature < 0.00001 && !pathChanged) { // if the temperature is too low and the path hasn't changed in the last iteration, restart the algorithm
            auto rd2 = std::random_device{};
            auto rand = std::default_random_engine{rd2()};
            std::ranges::shuffle(path, rand); // shuffle the path
            temperature = calculateStartingTemperature(); // set temperature to the starting temperature
            currentCost = pathCost(path);
        }

        std::vector<int> newPath = path;
        pathChanged = false;
        for (int i = 0; i <= (numberOfCities*4); i++) { // lentgth of epoch

            const int swapIndex1 = index(mt);
            int swapIndex2;
            do {
                swapIndex2 = index(mt);
            } while (swapIndex1 == swapIndex2); // swapIndex1 and swapIndex2 must be different
            std::swap(newPath[swapIndex1], newPath[swapIndex2]); // swap two cities

            int newCost = pathCost(newPath); // calculate the cost of the new path
            if (newCost < currentCost) { // if the new path is better, accept it
                path = newPath;
                currentCost = newCost;
                pathChanged = true;

                if (newCost < std::get<0>(bestSolution)) { // if the new path is the best so far, save it
                    bestSolution = {newCost, newPath, std::chrono::steady_clock::now() - start};
                    epochValuesAndTimes.push_back(std::to_string(newCost) + "," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(bestSolution)).count()));
                }
            } else if (exp((currentCost - newCost) / temperature) > (dist(mt))) { // if the new path is worse, accept it with a probability
                path = newPath;
                currentCost = newCost;
                pathChanged = true;
            }
        }
        temperature *= coolingRate; // cool the temperature
        if (temperature < lowestTemperature) { // save the lowest temperature
            lowestTemperature = temperature;
        }
        timeElapsed = std::chrono::steady_clock::now() - start; // calculate the time elapsed
    }

    return bestSolution;
}

std::tuple<int, std::vector<int>, std::chrono::duration<float>> simulatedAnnealing::simulatedAnnealingAlgorithm() { // return the best path and its cost
    auto result = findShortestPath();
    std::get<1>(result).push_back(std::get<1>(result)[0]); // add the starting city to the end of the path
    return result;
}






