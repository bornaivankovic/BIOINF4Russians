#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "tBlock.hpp"


using namespace std;

class fourRussians{
    public:
        int t;
        string hString;
        string vString;
        vector<vector<int>> permsO;
        vector<vector<char>> permsS;
        unordered_map<string, tBlock> blockMap;
        vector< vector<string> > dTable;
        vector<string> hSubS;
        vector<string> vSubS;
        

        fourRussians(int t, string hstring, string vstring); 

        void permutateO(vector<int> posOffset,vector<int> prefix, int length);
        void permutateS(vector<char> posOffset,vector<char> prefix, int length);
        void generateTBlocks();
        void fillDTable();
        void printDTable();
        int getMinDistance();
};

