//
// Created by Michał Zychowicz on 03/12/2023.
//

#include "greedy.h"

greedy::greedy(const std::vector<std::vector<int>> &matrix) : matrix(matrix) {
    numberOfCities = static_cast<int>(matrix.size());
    visited.resize(numberOfCities, false);
}

std::tuple<int, std::vector<int>> greedy::findShortestPath()  {
    std::vector<int> path;
    int totalCost = 0;
    int currentCity = 0;

    path.push_back(0);

    for (int i = 0; i < numberOfCities; i++) {
        visited[currentCity] = true;
        int nextCity = -1;
        int minCost = INT_MAX;

        for (int j = 0; j < numberOfCities; j++) {
            if (!visited[j] && matrix[currentCity][j] < minCost) {
                minCost = matrix[currentCity][j];
                nextCity = j;
            }
        }

        if (nextCity != -1) {
            totalCost += minCost;
            currentCity = nextCity;
            path.push_back(currentCity);
        }
    }

    // Return to the starting city
    totalCost += matrix[currentCity][0];
    path.push_back(0);

    return {totalCost, path};
}
