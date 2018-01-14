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
        vector<vector<int>> perms;
        unordered_map<string, tBlock> blockMap;
        

        fourRussians(int t, string hstring, string vstring); 

        void permutate(vector<int> posOffset,vector<int> prefix, int length);
        void generateTBlocks();
};

