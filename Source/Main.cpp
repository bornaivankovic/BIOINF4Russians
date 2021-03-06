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

//authors = Josipa
//Main method for executing algorithm
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

    //do the algorithm
    fourRussians fr = fourRussians(tBlockSize, firstString, secondString, outputFile);

    //generate T-Blocks and measure time needed to do so
    cout << "Generating T-Blocks:" << endl;
    auto start = chrono::high_resolution_clock::now();  
    fr.generateTBlocks();
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = finish - start;
    cout << "T-Blocks generated!" << endl;
    cout << "Duration of T-Blocks generation: " << elapsed1.count() << " s\n";
    
    //fill D-Table and measure time needed to do so
    cout << "Filling D-Table:" << endl;
    start = chrono::high_resolution_clock::now();
    fr.fillDTable();
    finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed2 = finish - start;
    cout << "D-Table filled!" << endl;
    cout << "Duration of D-Table filling: " << elapsed2.count() << " s\n";

    //print minimal edit distance
    cout << "Min edit distance: " << fr.getMinDistance() << endl;

    //make edit script and measure time needed to do so
    cout << "Making edit script:" << endl;
    start = chrono::high_resolution_clock::now();
    fr.makeEditScript();
    finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed3 = finish - start;
    cout << "Edit script made!" << endl;
    cout << "Duration of script making: " << elapsed3.count() << " s\n";

    //calculate memory and time 
    chrono::duration<double> elapsed = elapsed1 + elapsed2 + elapsed3;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
    double memory = fr.dTable.size()*fr.dTable[0].size()*sizeof(tBlock*)
        + fr.blockMap.size() * sizeof(tBlock);
    cout << "Memmory needed: " << ((memory / 1024) / 1024) << " MB" << endl;

    return 0;
}