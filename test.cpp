#include <iostream>
#include <string>
#include <vector>
#include "Source/tBlock.hpp"
#include "Source/fourRussians.hpp"

using namespace std;


int main(){
    
    int arr1[]={0,0,1,1},arr2[]={0,1,1,1};
    vector<int> b(arr1,arr1+sizeof(arr1)/sizeof(int)),c(arr2,arr2+sizeof(arr2)/sizeof(int));
    tBlock bl=tBlock("cbac","baba",b,c);
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
    fourRussians fr=fourRussians(2,"cbac","baba");
    fr.generateTBlocks("cbac","baba",5);

    cout<<endl;
    return 0;
}