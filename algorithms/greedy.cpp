
#include "greedy.h"

greedy::greedy(const std::vector<std::vector<int>> &matrix) : matrix(matrix) {
    numberOfCities = static_cast<int>(matrix.size());
    visited.resize(numberOfCities, false);
}

std::tuple<int, std::vector<int>> greedy::findShortestPath()  {
    std::vector<int> path;
    int totalCost = 0;
    int currentCity = 0; // start from city 0

    path.push_back(0);

    for (int i = 0; i < numberOfCities; i++) {
        visited[currentCity] = true; // mark current city as visited
        int nextCity = -1;
        int minCost = INT_MAX;

        for (int j = 0; j < numberOfCities; j++) {
            if (!visited[j] && matrix[currentCity][j] < minCost) { // find the next city with the minimum cost
                minCost = matrix[currentCity][j];
                nextCity = j;
            }
        }

        if (nextCity != -1) { // if there is a next city
            totalCost += minCost; // add the cost to the total cost
            currentCity = nextCity; // move to the next city
            path.push_back(currentCity); // add the next city to the path
        }
    }

    // Return to the starting city
    totalCost += matrix[currentCity][0];
    path.push_back(0);

    return {totalCost, path}; // return the total cost and the path
}
