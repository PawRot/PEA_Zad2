#include "dataGenerator.h"

using std::vector;

vector<vector<int>>
dataGenerator::generateTestData(unsigned int numberOfCities, unsigned int maxDistance, unsigned int minimumDistance) {
    vector<vector<int>> tab(numberOfCities, vector<int>(numberOfCities));

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(static_cast<int>(minimumDistance), static_cast<int>(maxDistance));

    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            if (i == j) {
                tab[i][j] = 0;
            } else {
                tab[i][j] = dist(mt);
            }
        }
    }
    return tab;
}
