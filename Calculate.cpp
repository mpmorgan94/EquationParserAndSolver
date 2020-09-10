#include "Calculate.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

Calculate::Calculate(string filename)
{

    resetStacks();

    inFileStream.open(filename);
    outFileStream.open("results.txt");

    loadPriorityTableMap();

    while (!inFileStream.eof())
    {
       readFile(filename); 
    }

}

//Validates the file
void Calculate::readFile(string filename)
{
    
    //Bool to mark if the equation is valid
    bool isValid = true;

    //Check that file opens
    if(!inFileStream.is_open())
    {
        cout << "Input file failed to open." << endl;
        throw "Input file failed to open.";
    }

    //Get next line of input file
    string equation = "";
    getline(inFileStream, equation);
      
    // stringstream class check1 
    stringstream check1(equation);
      
    string intermediate;
      
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    } 
    
    //Loop through the vector of tokens and handle them
    while (!tokens.empty() || value.size() > 2)
    {

        int originalSizeOfOpStack = op.size();
        bool currentIsOperator = isOperator(tokens[0]);
        bool hasPopAndProcessed = false;

        //Check what the current token is and handle accordingly
        if (tokens.empty())
        {
            popAndProcess();
        }
        else if (!currentIsOperator)
        {
            value.push(tokens[0]);
            tokens.erase(tokens.begin());
        }
        else if (currentIsOperator)
        {

            if (inputPriorityTable[tokens[0]] > priority.top())
            {
                op.push(tokens[0].at(0));
                priority.push(stackPriorityTable[tokens[0]]);
                tokens.erase(tokens.begin());
            }
            else
            {
                popAndProcess();
                hasPopAndProcessed = true;
            }
            
        }

        //If popandprocess didnt do anything, mark invalid
        if (originalSizeOfOpStack == op.size() && hasPopAndProcessed == true)
        {
            isValid = false;
            while (!tokens.empty())
            {
                tokens.erase(tokens.begin());
            }
            while (value.top() != "$")
            {
                value.pop();
            }
        }

    }

    //Check if equation was solved successfully
    if (op.top() != '$' && value.top() != "$")
    {
        isValid = false;
    }
    if (isValid == true)
    {
        writesResults(value.top());     
    }
    else
    {
        writesResults("invalid");
    }

}

//Writes results to file
void Calculate::writesResults(string answer)
{

    //Check if results file is open
    if (!outFileStream.is_open())
    {
        cout << "Failed to open results file." << endl;
        throw "Failed to open results file.";
    }

    //If equation was invalid, otherwise print answer
    if (answer == "$")
    {
        //Do nothing
    }
    else if (answer != "invalid" && fmod(stod(answer),1.0) == 0)
    {
        outFileStream << stoi(answer) << endl;  
    }
    else
    {
        outFileStream << answer << endl;
    }

    if (op.top() == '$')
    {
        while (!tokens.empty())
        {
            tokens.erase(tokens.begin());
        }
    }

    //Reset the stacks to prepare for next equation
    resetStacks();

}

//Where calculations are done
void Calculate::popAndProcess()
{
    
    //Check if theres enough values to process
    if (value.size() < 3)
    {
        //do nothing
    }
    //Handle parentheses
    else if (!tokens.empty() && tokens[0] == ")")
    {
        tokens.erase(tokens.begin());

        while (op.top() != '$' && op.top() != '(')
        {

            if (op.top() == '$')
            {
                writesResults("invalid");
            }
            else
            {
                double a = stod(value.top());
                value.pop();
                double b = stod(value.top());
                value.pop();

                double newValue = findNewValue(a, b, op.top());
                op.pop();
                priority.pop();

                value.push(to_string(newValue));
            }

        }
        if (op.top() == '(')
        {
            op.pop();
            priority.pop();
        }
    }
    //Handle all other possibilities
    else
    {

        if (op.top() == '$')
        {
            writesResults("invalid");
        }
        else
        {
            double a = stod(value.top());
            value.pop();
            
            double b = stod(value.top());
            value.pop();

            double newValue = findNewValue(a, b, op.top());

            op.pop();
            priority.pop();

            value.push(to_string(newValue));
        }

    }
    
}

//Loads the priority tables
void Calculate::loadPriorityTableMap()
{
    inputPriorityTable.insert(pair<string, int>("+", 1));
    stackPriorityTable.insert(pair<string, int>("+", 1));

    inputPriorityTable.insert(pair<string, int>("-", 1));
    stackPriorityTable.insert(pair<string, int>("-", 1));

    inputPriorityTable.insert(pair<string, int>("*", 2));
    stackPriorityTable.insert(pair<string, int>("*", 2));

    inputPriorityTable.insert(pair<string, int>("/", 2));
    stackPriorityTable.insert(pair<string, int>("/", 2));

    inputPriorityTable.insert(pair<string, int>("%", 2));
    stackPriorityTable.insert(pair<string, int>("%", 2));

    inputPriorityTable.insert(pair<string, int>("^", 5));
    stackPriorityTable.insert(pair<string, int>("^", 4));

    inputPriorityTable.insert(pair<string, int>("(", 6));
    stackPriorityTable.insert(pair<string, int>("(", 0));

    inputPriorityTable.insert(pair<string, int>(")", -99));
    stackPriorityTable.insert(pair<string, int>(")", -99));

    inputPriorityTable.insert(pair<string, int>("$", -99));
    stackPriorityTable.insert(pair<string, int>("$", -1));

    inputPriorityTable.insert(pair<string, int>("eoe", -99));
    stackPriorityTable.insert(pair<string, int>("eoe", -99));
}

//Checks if the parameter is an operator or number
bool Calculate::isOperator(string tokenToCheck)
{
    if (tokenToCheck == "+" || tokenToCheck == "-"
        || tokenToCheck == "*" || tokenToCheck == "/"
        || tokenToCheck == "%" || tokenToCheck == "^"
        || tokenToCheck == "(" || tokenToCheck == ")"
        || tokenToCheck == "$" || tokenToCheck == "eoe")
        {
            return true;
        }
    else
    {
        return false;
    }
}

//Computes the new value and return that value
double Calculate::findNewValue(double a, double b, char op)
{
    
    if (op == '+')
    {
        return b + a;
    }
    else if (op == '-')
    {
        return b - a;
    }
    else if (op == '*')
    {
        return b * a;
    }
    else if (op == '/')
    {
        return b / a;
    }
    else if (op == '%')
    {
        return fmod(b,a);
    }
    else if (op == '^')
    {
        return pow(b, a);
    }
    else
    {
        cout << "Invalid Operation." << endl;
        throw "Invalid Operation.";
    }

}

//Sets stacks to initial null values
void Calculate::resetStacks()
{

    while (!op.empty())
    {
        op.pop();
    }
    while (!priority.empty())
    {
        priority.pop();
    }
    while (!value.empty())
    {
        value.pop();
    }

    op.push('$');
    priority.push(-1);
    value.push("$");

}