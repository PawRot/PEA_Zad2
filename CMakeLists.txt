cmake_minimum_required(VERSION 3.26)
project(PEA_Zad2)


if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif(WIN32)

set(CMAKE_CXX_STANDARD 20)

add_executable(PEA_Zad2 menu/main.cpp
        data/dataGenerator.cpp
        data/dataGenerator.h
        data/fileOperator.cpp
        data/fileOperator.h
        rapidXML/rapidxml.hpp
        rapidXML/rapidxml_iterators.hpp
        rapidXML/rapidxml_print.hpp
        rapidXML/rapidxml_utils.hpp
        algorithms/simulatedAnnealing.cpp
        algorithms/simulatedAnnealing.h
        algorithms/greedy.cpp
        algorithms/greedy.h)
