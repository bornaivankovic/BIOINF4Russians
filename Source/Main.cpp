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
#include <chrono>

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
    }
    else
    {
        cout << "Cannot open file " << inputFile << endl;
    }
    fourRussians fr = fourRussians(tBlockSize, firstString, secondString, outputFile);

    cout << "Generating T-Blocks:" << endl;
    auto start = chrono::high_resolution_clock::now();
    
    fr.generateTBlocks();
    
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = finish - start;
    cout << "T-Blocks generated!" << endl;
    cout << "Duration of T-Blocks generation: " << elapsed1.count() << " s\n";
    
    cout << "Filling D-Table:" << endl;
    start = chrono::high_resolution_clock::now();
    
    fr.fillDTable();
    
    finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = finish - start;
    cout << "D-Table filled!" << endl;
    cout << "Duration of D-Table filling: " << elapsed2.count() << " s\n";

    cout << "Min edit distance: " << fr.getMinDistance() << endl;
    double memory = fr.dTable.size()*fr.dTable[0].size()*sizeof(tBlock*)
        + fr.blockMap.size() * sizeof(tBlock);
    cout << "Memmory needed: " << ((memory / 1024) / 1024) << " MB" << endl;

    chrono::duration<double> elapsed = elapsed1 + elapsed2;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
    
    return 0;
}