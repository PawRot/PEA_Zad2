#include <iostream>
#include <thread>
#include "../data/fileOperator.h"
#include "../data/dataGenerator.h"
#include "../algorithms/greedy.h"
#include "../algorithms/simulatedAnnealing.h"
// #include "../tests/tester.h"
using std::vector, std::string;

void showMenuOptions();

vector<vector<int>> loadFromFile(bool &dataLoaded);

vector<vector<int>> generateData(bool &dataLoaded);

void displayCurrentData(const vector<vector<int>> &data);

int setStopCriterion(bool &stopCriterionSet);

void setTempChangeFactor(long double &tempChangeFactor);

void startSimulatedAnnealing(const vector<vector<int>> &testData, vector<int> &path, const long double &tempChangeFactor, const int &stopCriterion, bool &pathLoaded);

void savePathToFile(const vector<int> &path);

vector<int> loadPathFromFile(bool &pathLoaded);

void calculateCost(const vector<vector<int>> &testData, const vector<int> &path);


int main(int argc, char **argv) {
    if (argc > 1 && std::string(argv[1]) == "testMode"){ // enter test mode

        auto data = fileOperator::loadXMLDataFromFile(R"(E:\Repozytoria\PEA_Zad2\data\rbg358.xml)");

        greedy greedy(data);
        const auto greedyResult = greedy.findShortestPath();

        static std::vector<double> coolingRates = {};
//        for (int i = 4; i < 10; ++i){
//            coolingRates.push_back(i*0.1);
//        }
//        coolingRates.push_back(0.99);
//        coolingRates.push_back(0.999);
//        coolingRates.push_back(0.99999);

        coolingRates.push_back(0.99999);
//        coolingRates.push_back(0.999999);

        for (auto element : coolingRates) {
            std::cout << element << std::endl;
        }

        int bestKnownCost = 1163;

        int bestPathCost = INT_MAX;
        int bestPathCostOverAll = INT_MAX;
        std::vector<string> epochValuesAndTimes = {};
        vector<int> bestPath = {};

        std::cout << "Testing file: rbg358.xml" << std::endl;
        for (const auto coolingRate : coolingRates) {
            std::cout << "Testing cooling rate: " << coolingRate << std::endl;
            int averagePathCost = 0;
            for (int i = 0; i < 10; ++i) {
                std::cout << "Test number: " << i << std::endl;
                simulatedAnnealing simulatedAnnealing(data, coolingRate, 360, greedyResult);
                const auto result = simulatedAnnealing.simulatedAnnealingAlgorithm();
                averagePathCost += std::get<0>(result);
                if (std::get<0>(result) < bestPathCost){
                    bestPathCost = std::get<0>(result);
                    bestPath = std::get<1>(result);
                    epochValuesAndTimes = simulatedAnnealing.epochValuesAndTimes;
                }
            }
            averagePathCost /= 10;
            double percentageError = (static_cast<double>(averagePathCost) / static_cast<double>(bestKnownCost))*100;
            fileOperator::saveResultFile("rbg358_test7.csv", {static_cast<int>(coolingRate*100000), averagePathCost, bestPathCost, static_cast<int>(percentageError*100000)});
            // saveResultFile works on integers only, so i multiply by 100000 and then divide by 100000 to get 5 decimal places
            if (bestPathCost < bestPathCostOverAll){
                bestPathCostOverAll = bestPathCost;
                fileOperator::savePathToFile("rbg358_test7_path.txt", bestPath);
                fileOperator::saveEpochsToFile("rbg358_test7_epoch.csv", epochValuesAndTimes);
            }
            bestPathCost = INT_MAX;
        }
        exit(0);
    }


    // normal execution entry point
    int choice = -1;
    string input;
    bool dataLoaded = false;
    bool stopCriterionSet = false;
    int stopCriterion = 0;
    bool pathLoaded = false;
    long double tempChangefactor = 0.99999;
    vector<vector<int>> testData;
    vector<int> path;

    do {
        std::cout << "Current temperature change factor: " << tempChangefactor << std::endl;
        if (stopCriterionSet) {
            std::cout << "Current stop criterion: " << stopCriterion << " seconds" << std::endl;
        } else {
            std::cout << "Stop criterion not set" << std::endl;
        }

        showMenuOptions();
        std::cin >> input;

        // wybór opcji menu
        try {
            choice = std::stoi(input);
        } catch (std::invalid_argument &e) {
            std::cout << "Invalid argument" << std::endl << std::endl;
            choice = -1;
        }


        switch (choice) {
            case 1:
                std::cout << std::endl;
                testData = loadFromFile(dataLoaded);
                std::cout << std::endl;
                break;
            case 2:
                std::cout << std::endl;
                testData = generateData(dataLoaded);
                std::cout << std::endl;
                break;
            case 3:
                std::cout << std::endl;
                if (dataLoaded) {
                    displayCurrentData(testData);
                } else {
                    std::cout << "No data loaded" << std::endl;
                }
                std::cout << std::endl;
                break;
            case 4:
                std::cout << std::endl;
                stopCriterion = setStopCriterion(stopCriterionSet);
                std::cout << std::endl;
                break;
            case 5:
                std::cout << std::endl;
                setTempChangeFactor(tempChangefactor);
                std::cout << std::endl;
                break;
            case 6:
                if (dataLoaded && stopCriterionSet) {
                    startSimulatedAnnealing(testData, path, tempChangefactor, stopCriterion, pathLoaded);
                } else {
                    std::cout << "No data loaded or no stop criteria set" << std::endl;
                }
                std::cout << std::endl;
                break;
            case 7:
                std::cout << std::endl;
                if (pathLoaded) {
                    savePathToFile(path);
                } else {
                    std::cout << "No path loaded" << std::endl;
                }
                std::cout << std::endl;
                break;
            case 8:
                std::cout << std::endl;
                path = loadPathFromFile(pathLoaded);
                std::cout << std::endl;
                break;
            case 9:
                std::cout << std::endl;
                if (dataLoaded && pathLoaded) {
                    calculateCost(testData, path);
                    std::cout << std::endl;
                } else {
                    std::cout << "No data loaded or no path loaded" << std::endl;
                }
                std::cout << std::endl;
                break;
            default:
                break;
        }
    } while (choice != 0);
}

void showMenuOptions() {
    std::cout << "1. Load test data from file" << std::endl;
    std::cout << "2. Generate test data" << std::endl;
    std::cout << "3. Display current test data" << std::endl;
    std::cout << "4. Set stop criteria" << std::endl;
    std::cout << "5. Set temperature change factor" << std::endl;
    std::cout << "6. Start Simulated Annealing" << std::endl;
    std::cout << "7. Save path to file" << std::endl;
    std::cout << "8. Load path from file" << std::endl;
    std::cout << "9. Calculate cost based on loaded path and test data" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

vector<vector<int>> loadFromFile(bool &dataLoaded) {
    std::cout << "Enter file path: ";
    string filePath;
//    std::cin >> filePath;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, filePath);

    std::cout << "Loading data from file: " << filePath << std::endl;
    // auto data = fileOperator::loadDataFromFile(filePath);
    auto data = fileOperator::loadXMLDataFromFile(filePath);
    if (!data.empty() || !data[0].empty()) {
        std::cout << "Data loaded successfully" << std::endl;
        dataLoaded = true;
        return data;
    } else {
        std::cout << "Data not loaded" << std::endl;
        dataLoaded = false;
        return {};
    }

}

vector<vector<int>> generateData(bool &dataLoaded) {
    std::cout << "Enter number of cities: ";
    string input;
    std::cin >> input;
    unsigned int numberOfCities;
    try {
        numberOfCities = std::stoi(input);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument" << std::endl;
        dataLoaded = false;
        return {};
    }

    if (numberOfCities < 2){
        std::cout << "Number of cities must be greater than 1" << std::endl;
        dataLoaded = false;
        return {};
    }

    std::cout << "Enter minimum distance between cities: ";
    unsigned int minimumDistance;
    std::cin >> input;

    try {
        minimumDistance = std::stoi(input);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument" << std::endl;
        dataLoaded = false;
        return {};
    }

    std::cout << "Enter maximum distance between cities: ";
    unsigned int maximumDistance;
    std::cin >> input;

    try {
        maximumDistance = std::stoi(input);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument" << std::endl;
        dataLoaded = false;
        return {};
    }
    std::cout << "Generating test data" << std::endl;
    auto data = dataGenerator::generateTestData(numberOfCities, maximumDistance, minimumDistance);
    if (!data.empty()) {
        std::cout << "Data generated successfully" << std::endl;
        dataLoaded = true;
        return data;
    } else {
        std::cout << "Data not generated" << std::endl;
        dataLoaded = false;
        return {};
    }

}

void displayCurrentData(const vector<vector<int>> &data) {
    const int numberOfCities = static_cast<int>(data.size());
    std::cout << "Current number of cities: " << numberOfCities << std::endl;
    std::cout << "Current test data matrix: " << std::endl;
    for (int i = 0; i < numberOfCities; ++i) {
        for (int j = 0; j < numberOfCities; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int setStopCriterion(bool &stopCriterionSet) {
    std::cout << "Enter stop criterion in seconds: ";
    string input;
    std::cin >> input;
    int stopCriterion;
    try {
        stopCriterion = std::stoi(input);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument" << std::endl;
        stopCriterionSet = false;
        return -1;
    }

    if (stopCriterion < 1) {
        std::cout << "Stop criterion must be greater than 0" << std::endl;
        stopCriterionSet = false;
        return -1;
    }

    std::cout << "Stop criterion set to: " << stopCriterion << std::endl;
    stopCriterionSet = true;
    return stopCriterion;
}

void setTempChangeFactor(long double &tempChangeFactor) {
    std::cout << "Enter temperature change factor: ";
    string input;
    std::cin >> input;
    long double tempChangeFactorInput;
    try {
        tempChangeFactorInput = std::stod(input);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument" << std::endl;
        return;
    }

    if (tempChangeFactorInput < 0 || tempChangeFactorInput >= 1) {
        std::cout << "Temperature change factor must be greater than 0 and lower than 1" << std::endl;
        return;
    }

    std::cout << "Temperature change factor set to: " << tempChangeFactorInput << std::endl;
    tempChangeFactor = tempChangeFactorInput;
}

void startSimulatedAnnealing(const vector<vector<int>>&testData, vector<int>&path, const long double &tempChangeFactor, const int &stopCriterion, bool &pathLoaded){
    std::cout << "Starting Simulated Annealing" << std::endl;
    std::cout << std::endl;
    greedy greedy(testData);
    const auto greedyResult = greedy.findShortestPath();
    std::cout << "Best greedy path found: " << std::endl;
    for (const auto element : std::get<1>(greedyResult)){
        std::cout << element << " ";
    }
    std::cout << std::endl;
    std::cout << "Cost of greedy path: " << std::get<0>(greedyResult) << std::endl;

    simulatedAnnealing simulatedAnnealing(testData, tempChangeFactor, stopCriterion, greedyResult);

    const auto result = simulatedAnnealing.simulatedAnnealingAlgorithm();

    // auto result = simulatedAnnealing::simulatedAnnealingAlgorithm(testData, tempChangeFactor, stopCriterion, greedyResult);
    std::cout << "Best path found: " << std::endl;
    for (const auto element : std::get<1>(result)){
    std::cout << element << " ";
    }
    std::cout << std::endl;
    std::cout << "Cost of path: " << std::get<0>(result) << std::endl;
    std::cout << "Best path found after: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::get<2>(result)).count() << " milliseconds" << std::endl;
    path = std::get<1>(result);
    pathLoaded = true;
    const auto endTemperature = simulatedAnnealing.getTemperature();
    std::cout << "Temperature when execution ended: " << endTemperature << std::endl;
    const auto exponent = exp((-1/endTemperature));
    std::cout << "exp(-1/T_k): " << exponent << std::endl;
    std::cout << "Lowest temperature: " << simulatedAnnealing.getLowestTemperature() << std::endl;
    std::cout << "exp(-1/T_min): " << exp((-1/simulatedAnnealing.getLowestTemperature())) << std::endl;

}

void savePathToFile(const vector<int> &path) {
    std::cout << "Enter file path with extension: ";
    string filePath;
//    std::cin >> filePath;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, filePath);

    std::cout << "Saving path to file: " << filePath << std::endl;
    fileOperator::savePathToFile(filePath, path);
}

vector<int> loadPathFromFile(bool&pathLoaded) {
    std::cout << "Enter file path: ";
    string filePath;
//    std::cin >> filePath;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, filePath);

    std::cout << "Loading data from file: " << filePath << std::endl;
    auto data = fileOperator::loadPathFromFile(filePath);
    if (!data.empty()) {
        std::cout << "Data loaded successfully" << std::endl;
        pathLoaded = true;
        // for (auto element : data){
        //     std::cout << element << " ";
        // }
        return data;
    } else {
        std::cout << "Data not loaded" << std::endl;
        pathLoaded = false;
        return {};
    }
}

void calculateCost(const vector<vector<int>>&testData, const vector<int>&path) {

    int cost = 0;

    std::cout << "Path: ";
    for (const auto element : path){
        std::cout << element << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < path.size() - 1; ++i) {
        cost += testData[path[i]][path[i + 1]];
    }

    std::cout << "Cost of path: " << cost;

}






