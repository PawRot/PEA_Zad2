
#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include <algorithm>



class greedy {

private:
    std::vector<std::vector<int>> matrix;
    std::vector<bool> visited;
    int numberOfCities;

public:
    explicit greedy(const std::vector<std::vector<int>>& matrix);

    std::tuple<int , std::vector<int>> findShortestPath();

};



#endif //GREEDY_H
