#include  "Calculate.h"
#include <iostream>
#include <cmath>

using namespace std;

void printGreeting()
{
    cout << "John Morgan Patranella" << endl;
    cout << "Section: 501" << endl;
}

int main(int argc, char *argv[])
{

    //Check for correct number of needed arguments
    if (argc != 2)
    {
        cout << "Invalid number of arguments." << endl;
        throw "Argument Error.";
    }

    string fileToRead = argv[1];

    printGreeting();

    Calculate calc(fileToRead);
    
}