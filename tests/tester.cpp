
#include "tester.h"

#include <iostream>

using std::string, std::vector, std::cout;

void tester::run_tests(std::string test_type) {
    std::vector<std::vector<int>> data = {};

        typedef std::tuple<int, vector<int>> greedyResultType;

        greedyResultType greedyResult;

        static std::vector<long double> coolingRates = {0.999, 0.9999, 0.99999};

        for (auto element : coolingRates) {
            std::cout << element << std::endl;
        }


        int bestKnownCost;
        int stopCriterion = 1;
        string fileNameTest;
        string fileNameEpoch;
        string fileNamePath;
        string fileNameTemperature;
        if (std::string(test_type) == "1") {
            data = fileOperator::loadXMLDataFromFile(R"(E:\Repozytoria\PEA_Zad2\data\ftv55.xml)");
            greedy greedy(data);
            greedyResult = greedy.findShortestPath();
            bestKnownCost = 1608;
            stopCriterion = 120;
            fileNameTest = "ftv55_results.csv";
            fileNameEpoch = "ftv55_epoch.csv";
            fileNamePath = "ftv55_path.txt";
            fileNameTemperature = "ftv55_temperature.csv";
            std::cout << "Testing file: ftv55.xml" << std::endl;
        } else if (std::string(test_type) == "2") {
            data = fileOperator::loadXMLDataFromFile(R"(E:\Repozytoria\PEA_Zad2\data\ftv170.xml)");
            greedy greedy(data);
            greedyResult = greedy.findShortestPath();
            bestKnownCost = 2755;
            stopCriterion = 240;
            fileNameTest = "ftv170_results.csv";
            fileNameEpoch = "ftv170_epoch.csv";
            fileNamePath = "ftv170_path.txt";
            fileNameTemperature = "ftv170_temperature.csv";
            std::cout << "Testing file: ftv170.xml" << std::endl;
        } else if (std::string(test_type) == "3") {
            data = fileOperator::loadXMLDataFromFile(R"(E:\Repozytoria\PEA_Zad2\data\rbg358.xml)");
            greedy greedy(data);
            greedyResult = greedy.findShortestPath();
            bestKnownCost = 1163;
            stopCriterion = 360;
            fileNameTest = "rbg358_results.csv";
            fileNameEpoch = "rbg358_epoch.csv";
            fileNamePath = "rbg358_path.txt";
            fileNameTemperature = "rbg358_temperature.csv";
            std::cout << "Testing file: rbg358.xml" << std::endl;
        } else {
            std::cout << "Invalid second argument provided";
            exit(0);
        }


        int bestPathCost = INT_MAX;
        std::vector<string> epochValuesAndTimes = {};
        vector<int> bestPath = {};
        std::vector<string> temperatures = {};


        for (const auto coolingRate : coolingRates) {
            temperatures = {};
            std::cout << "Testing cooling rate: " << coolingRate << std::endl;
            for (int i = 0; i < 10; ++i) {
                std::cout << "Test number: " << i << std::endl;
                simulatedAnnealing simulatedAnnealing(data, coolingRate, stopCriterion, greedyResult);
                const auto result = simulatedAnnealing.simulatedAnnealingAlgorithm();

                const auto resultCost = std::get<0>(result);
                const auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(result)).count();

//                double percentageError = (static_cast<double>(resultCost) / static_cast<double>(bestKnownCost))*100;
                const double percentageError = (static_cast<double>(resultCost) - static_cast<double>(bestKnownCost)) / static_cast<double>(bestKnownCost);
                fileOperator::saveResultFile(std::to_string(coolingRate)+fileNameTest, {resultCost, timeElapsed, static_cast<int>(percentageError * 100000)});
                // saveResultFile works on integers only,
                // so I multiply by 100000 and then divide by 100000 to get 5 decimal places

                string temperaturesString = std::to_string(simulatedAnnealing.getTemperature()) + "," + std::to_string(simulatedAnnealing.getLowestTemperature());

                temperatures.push_back(temperaturesString);

                if (resultCost < bestPathCost){
                    bestPathCost = resultCost;
                    bestPath = std::get<1>(result);
                    epochValuesAndTimes = simulatedAnnealing.epochValuesAndTimes;
                }
            }

            fileOperator::savePathToFile(std::to_string(coolingRate)+fileNamePath, bestPath);
            fileOperator::saveEpochsToFile(std::to_string(coolingRate)+fileNameEpoch, epochValuesAndTimes);
            fileOperator::saveEpochsToFile(std::to_string(coolingRate)+fileNameTemperature, temperatures);

        }
}


