#include <iostream>
#include <string>
#include <vector>
#include "Source/tBlock.hpp"
#include "Source/fourRussians.hpp"
#include <sstream>
using namespace std;


int main(){
    
    int arr1[]={1,1},arr2[]={-1,-1};
    vector<int> b(arr1,arr1+sizeof(arr1)/sizeof(int)),c(arr2,arr2+sizeof(arr2)/sizeof(int));
    tBlock bl=tBlock("CC","CA",b,c);
    fourRussians fr=fourRussians(2,"ACACCC","CACAAA");
    fr.generateTBlocks();
    fr.fillDTable();
    fr.printDTable();
    cout<<"Min distance: "<<fr.getMinDistance();
    return 0;
}