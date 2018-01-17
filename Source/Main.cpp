#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>
#include "fourRussians.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string inputFile;
    string outputFile;
    int tBlockSize;

    //check the number of params
    if (argc != 4)
    {
        cout << argc;
        cout << "This program requires 3 arguments: <inputFile> <outputFile> <tBlockSize>";
        return 0;
    }

    // inputFile, outputFile and tBlockSize extracted from params
    try
    {
        inputFile = argv[1];
        outputFile = argv[2];
        tBlockSize = stoi(argv[3]);
    }
    catch (std::invalid_argument &e)
    {
        cout << "T-Block size must be a number!";
        return 0;
    }

    //strings for algorithm extracted form inputFIle
    string firstString;
    string secondString;

    //open inputFile and extract two strings
    ifstream inputStream(inputFile);
    if (inputStream.is_open())
    {

        getline(inputStream, firstString);
        getline(inputStream, secondString);

        inputStream.close();

        //remove \n 
        firstString.erase(firstString.size() - 1, 1);
        secondString.erase(secondString.size() - 1, 1);

        fourRussians fr = fourRussians(tBlockSize, firstString, secondString, outputFile);
        fr.generateTBlocks();
        fr.fillDTable();
        cout << "Min edit distance: " << fr.getMinDistance() << endl;
    }
    else
    {
        cout << "Cannot open file " << inputFile << endl;
    }

    return 0;
}