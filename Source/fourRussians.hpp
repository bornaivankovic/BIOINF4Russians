#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "tBlock.hpp"
#include "Hash.hpp"

using namespace std;

class fourRussians{
    public:
        int t;
        string hString;
        string vString;
        string outputFile;
        int minDistance;
        
        unordered_map<vector<int>,string,VectorHash> permsO;
        //vector<vector<int>> permsO;
        string *permsSP;
        //vector<vector<char>> permsS;
        unordered_map<string, tBlock> blockMap;
        vector< vector<tBlock*> > dTable;
        vector<string> hSubS;
        vector<string> vSubS;

        fourRussians(int t, string hString, string vString, string outputFile); 

        void permutateO(vector<int> posOffset,vector<int> prefix, int length);
        void permutateS(const char str[],string prefix,const int n, const int lenght, int* k);
        void generateTBlocks();
        void fillDTable();
        void makeEditScript();
        int getMinDistance();
    
        
};

 

