#include "fileOperator.h"

using std::cout, std::endl, std::vector, std::string;

fileOperator::fileOperator() = default;

fileOperator::~fileOperator() = default;

void fileOperator::saveDataFile(const string &path, const vector<vector<int>> &data) {
    std::ofstream file;

    file.open(path, std::ios::out);

    if (file.good()) {
        cout << "File created successfully" << std::endl;
    } else {
        cout << "Failed to create file" << std::endl;
        return;
    }

    if(file.is_open()) {
        int size = static_cast<int>(data.size());
        file << size << std::endl;
        for (int i = 0; i < size; ++i){
            for (int j = 0; j < size; ++j){
                file << data[i][j] << " ";
            }
            file << std::endl;
        }
    }


}

vector<vector<int>> fileOperator::loadDataFromFile(const string &path) {
    std::ifstream file;
    file.open(path, std::ios::in);
    if (file.good()) {
        cout << "File loaded successfully" << endl;
    } else {
        cout << "File not found" << endl;
    }

    int numberOfCities;

    if (file.is_open()) {
        file >> numberOfCities;

        int distance;
        vector<vector<int>> tab(numberOfCities, vector<int>(numberOfCities));

        if (file.fail()) {
            cout << "File error" << endl;
        } else {
            for (int i = 0; i < numberOfCities; i++) {
                for (int j = 0; j < numberOfCities; j++) {
                    file >> distance;

                    if (file.fail()) {
                        cout << "File error" << endl;
                        break;
                    } else {
                        tab[i][j] = distance;
                    }
                }
            }
            file.close();
            return tab;
        }
    } else {
        cout << "File error" << endl;
    }
    file.close();
    return {};
}

std::vector<int> fileOperator::loadPathFromFile(const std::string& filePath) {
    std::ifstream file;
    file.open(filePath, std::ios::in);
    if (file.good()) {
        cout << "File loaded successfully" << endl;
    } else {
        cout << "File not found" << endl;
    }

    int numberOfCities;

    if (file.is_open()) {
        file >> numberOfCities;

        int city;
        std::vector<int> path(numberOfCities + 1);

        if (file.fail()) {
            cout << "File error" << endl;
        } else {
            for (int i = 0; i < numberOfCities + 1; i++) {
                file >> city;

                if (file.fail()) {
                    cout << "File error" << endl;
                    break;
                } else {
                    path[i] = city;
                }
            }
            file.close();
            return path;
        }
    } else {
        cout << "File error" << endl;
    }
    file.close();
    return {};
}

std::vector<std::vector<int>> fileOperator::loadXMLDataFromFile(const std::string& path) {

    int numberOfCities = 0;

    rapidxml::file<> xmlFile(path.c_str());


    rapidxml::xml_document<> document;

    try {
        document.parse<0>(xmlFile.data());
    } catch (const rapidxml::parse_error &e) {
        std::cout << "Parse error: " << e.what() << std::endl;
        return {};
    }

    // get root node
    rapidxml::xml_node<> *rootNode = document.first_node();
    // print name of root node
    std::cout << "Root node name: " << rootNode->name() << std::endl;

    rapidxml::xml_node<> *graphNode = rootNode->first_node("graph");

    for (rapidxml::xml_node<> *vertexNode = graphNode->first_node("vertex"); vertexNode; vertexNode = vertexNode->next_sibling()) {
        numberOfCities++;
    }

    std::vector<std::vector<int>> matrix(numberOfCities, std::vector<int>(numberOfCities, 0));

    int i = 0;
    for (rapidxml::xml_node<> *vertexNode = graphNode->first_node("vertex"); vertexNode; vertexNode = vertexNode->next_sibling(), i++) {
        for (rapidxml::xml_node<> *edgeNode = vertexNode->first_node("edge"); edgeNode; edgeNode = edgeNode->next_sibling()) {
            int j = std::stoi(edgeNode->value());
            double doubleCost = std::stod(edgeNode->first_attribute("cost")->value());
            int cost = static_cast<int>(doubleCost);
            matrix[i][j] = cost;
        }
    }


    return matrix;
}

void fileOperator::saveResultFile(const string &path, const vector<long long> &data) {
    std::ofstream file;
    file.open(path, std::ios::app);
    if (file.good()) {
        cout << "File opened successfully" << endl;
    } else {
        cout << "File not found" << endl;
    }

    if (file.is_open()) {
        int size = static_cast<int>(data.size());
        int k = 0;
        for (long long int i : data) {
            file  << i;
            if (k < size - 1) {
                file << ",";
            }
            k++;
        }
        file << endl;
        file.close();
    } else {
        cout << "File error - OPEN" << endl;
    }
}

void fileOperator::saveEpochsToFile(const std::string &filePath, const std::vector<std::string> &epochValuesAndTimes) {
    std::ofstream file;

    file.open(filePath, std::ios::out);

    if (file.good()) {
        cout << "File created successfully" << std::endl;
    } else {
        cout << "Failed to create file" << std::endl;
        return;
    }

    if(file.is_open()) {
        int size = static_cast<int>(epochValuesAndTimes.size());
        for (int i = 0; i < size; ++i){
            file << epochValuesAndTimes[i] << std::endl;
        }
    }
}

void fileOperator::savePathToFile(const std::string& filePath, const std::vector<int>& pathData) {
    std::ofstream file;

    file.open(filePath, std::ios::out);

    if (file.good()) {
        cout << "File created successfully" << std::endl;
    } else {
        cout << "Failed to create file" << std::endl;
        return;
    }

    if(file.is_open()) {
        int size = static_cast<int>(pathData.size());
        file << size-1 << std::endl;
        for (int i = 0; i < size; ++i){
                file << pathData[i] << " ";
            file << std::endl;
        }
    }


}

