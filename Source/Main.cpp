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

int main()
{

    using namespace std;

    cout << "Please provide following parameters:" << endl;
    cout << "<inputFile> <tBlockSize>" << endl;

    //cmd params
    string params;

    // inputFile and size extracted from params
    string inputFile;
    int tBlockSize;

    //strings for algorithm extracted form inputFIle
    string firstString;
    string secondString;

    while (true)
    {
        //get params form standard input
        getline(cin, params);

        //split them with delimiter " "
        istringstream iss(params);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

        //if number of params is not equal to 2, something is wrong
        if (tokens.size() != 2)
        {
            cout << "Please provide following parameters:" << endl;
            cout << "<inputFile> <tBlockSize>" << endl;
            continue;
        }

        //try to parse params & open inputFile
        try
        {
            inputFile = tokens.at(0);
            tBlockSize = stoi(tokens.at(1));
            
            ifstream inputStream;
            inputStream.open(inputFile);
            if(!inputStream.is_open())
                throw exception();

            inputStream.close();
        }
        catch (exception& e)
        {   
            cout << "Please provide following parameters:" << endl;
            cout << "<inputFile> <tBlockSize>" << endl;
            continue;
        }

        cout << "Thank you for providing necessary params!" << endl;
        break;
    }

    //open inputFile and extract two strings
    ifstream inputStream;
    inputStream.open(inputFile);

    inputStream.close();
}