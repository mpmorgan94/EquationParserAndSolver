#ifndef CALCULATE_H
#define CALCULATE_H

#include <string>
#include <stack>
#include <map>
#include <vector>
#include <fstream>

class Calculate
{

public:

    Calculate(){}
    Calculate(std::string filename);

    //Validates the file
    void readFile(std::string filename);

    //Writes results to file
    void writesResults(std::string answer);

    //Where calculations are done
    void popAndProcess();

    //Load Maps
    void loadPriorityTableMap();

    //Checks if token is an operator
    bool isOperator(std::string tokenToCheck);

    //Computes the new value and return that value
    double findNewValue(double a, double b, char op);

    //Resets stacks
    void resetStacks();


private:

    std::stack<char> op;
    std::stack<int> priority;
    std::stack<std::string> value;

    std::map<std::string, int> inputPriorityTable;
    std::map<std::string, int> stackPriorityTable;

    std::vector<double> answers;
    std::vector <std::string> tokens;

    std::ifstream inFileStream;
    std::ofstream outFileStream;

};


#endif