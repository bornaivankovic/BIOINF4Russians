#include <iostream>
#include <string>
#include <vector>
#include "Source/tBlock.hpp"
#include "Source/fourRussians.hpp"
#include <sstream>
using namespace std;


int main(){
    
    int arr1[]={0,0},arr2[]={0,0};
    vector<int> b(arr1,arr1+sizeof(arr1)/sizeof(int)),c(arr2,arr2+sizeof(arr2)/sizeof(int));
    tBlock bl=tBlock("cb","ba",b,c);
    cout<<"hOffsets: ";
    for(int i=0;i<bl.hOffsets.size();i++){
        cout<<bl.hOffsets[i]<<",";
    }
    cout<<"\n";
    cout<<"vOffsets: ";
    for(int i=0;i<bl.vOffsets.size();i++){
        cout<<bl.vOffsets[i]<<",";
    }

    cout<<"\n";
    fourRussians fr=fourRussians(2,"cbac","babb");
    fr.generateTBlocks();
    stringstream a;
    a<<"cb"<<"ba"<<"0000";
    tBlock tb=fr.blockMap[a.str()];
    cout<<"hOffsets: ";
    for(int i=0;i<tb.hOffsets.size();i++){
        cout<<tb.hOffsets[i]<<",";
    }
    cout<<"\n";
    cout<<"vOffsets: ";
    for(int i=0;i<tb.vOffsets.size();i++){
        cout<<tb.vOffsets[i]<<",";
    }

    cout<<endl;
    return 0;
}