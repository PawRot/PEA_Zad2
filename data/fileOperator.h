#ifndef PEA_ZAD2_FILEOPERATOR_H
#define PEA_ZAD2_FILEOPERATOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "../rapidXML/rapidxml.hpp"
#include "../rapidXML/rapidxml_utils.hpp"
#include "../rapidXML/rapidxml_print.hpp"



class fileOperator {
public:
    fileOperator();

    ~fileOperator();
    static void saveDataFile(const std::string &path, const std::vector<std::vector<int>> &data);
    static void saveResultFile(const std::string &path, const std::vector<long long> &data);
    static void savePathToFile(const std::string &filePath, const std::vector<int> &pathData);

    static void saveEpochsToFile(const std::string &filePath, const std::vector<std::string> &epochValuesAndTimes);

    static std::vector<std::vector<int>> loadDataFromFile(const std::string &path);
    static std::vector<int> loadPathFromFile(const std::string &filePath);

    static std::vector<std::vector<int>> loadXMLDataFromFile(const std::string &path);};


#endif //PEA_ZAD2_FILEOPERATOR_H
