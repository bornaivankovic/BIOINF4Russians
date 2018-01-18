#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class tBlock{
public:
    vector<int> hOffsets;
    vector<int> vOffsets;

    tBlock();
    tBlock(const string& x,const string& y,vector<int>& b,vector<int>& c);
    static vector<vector<int>> calcBody(int t,const string& x,const string& y,vector<int>& b,vector<int>& c);
};