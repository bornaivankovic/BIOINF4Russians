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

int main(int argc, char* argv[])
{
    cout << "Please provide following parameters:" << endl;
    cout << "<inputFile> <tBlockSize>" << endl;

    // inputFile and size extracted from params
    string inputFile;
    int tBlockSize;

    //strings for algorithm extracted form inputFIle
    string firstString;
    string secondString;

    inputFile=argv[1];
    tBlockSize=stoi(argv[2]);

    //open inputFile and extract two strings
    ifstream inputStream(inputFile);
    if(inputStream.is_open()){
        getline(inputStream,firstString);
        getline(inputStream,secondString);
        inputStream.close();
        firstString.erase(firstString.size()-1,1);
        secondString.erase(secondString.size()-1,1);
        cout<<firstString<<endl<<secondString<<endl;
        fourRussians fr=fourRussians(tBlockSize,firstString,secondString);
        cout<<"constructor done"<<endl;
        fr.generateTBlocks();
        cout<<"generate done"<<endl;
        //cout<<"n blocks "<<fr.blockMap.size()<<endl;;
        fr.fillDTable();
        cout<<"fill done"<<endl;
        //cout<<"Min edit distance: "<<fr.getMinDistance()<<endl;
    }
    else{
        cout<<"Cannot open file "<<inputFile<<endl;
    }
    return 0;
}