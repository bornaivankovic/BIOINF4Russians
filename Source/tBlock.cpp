#include "tBlock.hpp"
#include <iostream>

using namespace std;
tBlock::tBlock(){}
tBlock::tBlock(const string& x,const string& y,vector<int>& b,vector<int>& c){
    int t=x.size();

    //Prepare tBlock for calculation of its "body"
    vector< vector<int> > tmpBlock;
    tmpBlock.resize(t+1);
    for(int i=0;i<tmpBlock.size();i++){
        tmpBlock[i].resize(t+1);
    }
    tmpBlock[0][0]=0;
    for(int i=1;i<t+1;i++){
        tmpBlock[0][i]=tmpBlock[0][i-1]+b[i-1];
        tmpBlock[i][0]=tmpBlock[i-1][0]+c[i-1];
    }
    
    /*
    Calculate all values inside the tBlock using the recurrence relation
    v1 - diagonal value, v2 - left value, v3 - up value
    */
    int v1,v2,v3;
    for(int i=1;i<t+1;i++){
        for(int j=1;j<t+1;j++){
            if(x[j-1]==y[i-1]){
                v1=tmpBlock[i-1][j-1];
            }
            else{
                v1=tmpBlock[i-1][j-1]+1;
            }
            v2=tmpBlock[i][j-1]+1;
            v3=tmpBlock[i-1][j]+1;
            tmpBlock[i][j]=min({v1,v2,v3});
        }
    }

    //Store output offset vectors for future use
    for(int i=1;i<t+1;i++){
        hOffsets.push_back(tmpBlock[t][i]-tmpBlock[t][i-1]);
        vOffsets.push_back(tmpBlock[i][t]-tmpBlock[i-1][t]);
    }
}


