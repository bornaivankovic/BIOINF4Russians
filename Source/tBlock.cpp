#include "tBlock.hpp"
#include <iostream>

using namespace std;
tBlock::tBlock(){}
vector<vector<int>> tBlock::calcBody(int t,const string& x,const string& y,vector<int>& b,vector<int>& c){
    vector< vector<int> > block;
    block.resize(t+1);
    for(int i=0;i<block.size();i++){
        block[i].resize(t+1);
    }
    block[0][0]=0;
    for(int i=1;i<t+1;i++){
        block[0][i]=block[0][i-1]+b[i-1];
        block[i][0]=block[i-1][0]+c[i-1];
    }
    
    /*
    Calculate all values inside the tBlock using the recurrence relation
    v1 - diagonal value, v2 - left value, v3 - up value
    */
    int v1,v2,v3;
    for(int i=1;i<t+1;i++){
        for(int j=1;j<t+1;j++){
            if(x[j-1]==y[i-1]){
                v1=block[i-1][j-1];
            }
            else{
                v1=block[i-1][j-1]+1;
            }
            v2=block[i][j-1]+1;
            v3=block[i-1][j]+1;
            block[i][j] = min({v1,v2,v3});
            
        }
    }
    return block;
}
tBlock::tBlock(const string& x,const string& y,vector<int>& b,vector<int>& c){
    int t=x.size();

    //Prepare tBlock for calculation of its "body"
    vector< vector<int> > tmpBlock=calcBody(t,x,y,b,c);

    //Store output offset vectors for future use
    for(int i=1;i<t+1;i++){
        hOffsets.push_back(tmpBlock[t][i]-tmpBlock[t][i-1]);
        vOffsets.push_back(tmpBlock[i][t]-tmpBlock[i-1][t]);
    }
}




