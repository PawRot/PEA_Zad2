
#include <iostream>
#include "simulatedAnnealing.h"



simulatedAnnealing::simulatedAnnealing(const std::vector<std::vector<int>>&matrix, long double coolingRate, int stopCriterion, const std::tuple<int, std::vector<int>>&greedyResult)
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
    // int numberOfSwaps = numberOfCities * numberOfCities * 2;
    numberOfSwaps = numberOfSwaps > 15000 ? 15000 : numberOfSwaps;
    int sum = 0;

    for (int i = 0; i < numberOfSwaps; i++) {
        auto neighborPath = path;
        const int swapIndex1 = index(mt);
        int swapIndex2;
        do {
            swapIndex2 = index(mt);
        } while (swapIndex1 == swapIndex2);
        std::swap(neighborPath[swapIndex1], neighborPath[swapIndex2]);
        sum += abs(pathCost(path) - pathCost(neighborPath));
        // sum += (pathCost(path) - pathCost(neighborPath));
    }

    sum /= numberOfSwaps;
    // std::cout << sum << std::endl;
    // std::cout << (-1*sum)/log(0.99) << std::endl;
    return (-1*sum)/log(0.99);
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
    std::uniform_int_distribution index(0, numberOfCities - 1);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::tuple<int, std::vector<int>, std::chrono::duration<float>> bestSolution = {INT_MAX, {}, std::chrono::duration<float>::zero()};

    std::chrono::duration<float> timeElapsed = std::chrono::duration<float>::zero();
    const auto start = std::chrono::steady_clock::now();
    int counter = 0;
    bool pathChanged = false;
    while (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) < std::chrono::seconds(stopCriterion)) {

        if (temperature < 0.00001 && !pathChanged) {
            auto rd2 = std::random_device{};
            auto rand = std::default_random_engine{rd2()};
            std::shuffle(path.begin(), path.end(), rand);
            temperature = calculateStartingTemperature();
            currentCost = pathCost(path);
        }

        std::vector<int> newPath = path;
        pathChanged = false;
        for (int i = 0; i <= (numberOfCities*4); i++) {

            const int swapIndex1 = index(mt);
            int swapIndex2;
            do {
                swapIndex2 = index(mt);
            } while (swapIndex1 == swapIndex2);
            std::swap(newPath[swapIndex1], newPath[swapIndex2]);

            int newCost = pathCost(newPath);
            if (newCost < currentCost) {
                path = newPath;
                currentCost = newCost;
                pathChanged = true;

                if (newCost < std::get<0>(bestSolution)) {
                    counter++;
//                    std::cout << newCost << std::endl;
                    bestSolution = {newCost, newPath, std::chrono::steady_clock::now() - start};
                    epochValuesAndTimes.push_back(std::to_string(newCost) + "," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(bestSolution)).count()));
                }
            } else if (exp((currentCost - newCost) / temperature) > (dist(mt))) {
                path = newPath;
                currentCost = newCost;
                pathChanged = true;
            }
        }
        // std::cout << temperature << std::endl;
        temperature *= coolingRate;
        if (temperature < lowestTemperature) {
            lowestTemperature = temperature;
        }
        timeElapsed = std::chrono::steady_clock::now() - start;
    }

    // std::cout << std::endl << std::get<0>(bestPath) << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(bestPath)).count() << std::endl;
//    std::cout << "counter: " <<  counter << std::endl;
    return bestSolution;
}

std::tuple<int, std::vector<int>, std::chrono::duration<float>> simulatedAnnealing::simulatedAnnealingAlgorithm() {
    auto result = findShortestPath();
    std::get<1>(result).push_back(std::get<1>(result)[0]);
    return result;
}






