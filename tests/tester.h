
#ifndef TESTER_H
#define TESTER_H
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include "../data/fileOperator.h"
#include "../algorithms/greedy.h"
#include "../algorithms/simulatedAnnealing.h"


class tester {
public:
    void static run_tests(std::string test_type);

};



#endif //TESTER_H
