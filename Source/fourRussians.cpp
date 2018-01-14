#include "fourRussians.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "tBlock.hpp"
#include <unordered_map>
#include <sstream>

using namespace std;

fourRussians::fourRussians(int t, string hString, string vString){
    this->t=t;  
    this->hString=hString;
    this->vString=vString;
}

//Method for generating and storing tBlocks
void fourRussians::generateTBlocks(){

    //creating permutations of offset vectors (size t) 
    vector<int> posOffset { 0,-1,1 };
    vector<int> prefix;
    permutate(posOffset,prefix,t);
    
    //ako hocete isprobat ispis permutacija
    for(int i=0;i<perms.size();i++){
        for(int j=0;j<perms.at(i).size();j++){
            //cout<<perms.at(i).at(j);
        }
        //cout<<"\n";
    }
    //broj permutacija
    //cout<<perms.size();
    

    //creating t size vectors of input strings and generating tBlocks 
    int i=0;
    int j=0;
    vector<string> hv;
    vector<string> vv;
    
    while(i<hString.length()-(t-1)){
        hv.insert(hv.begin()+j,hString.substr(i,t));
        vv.insert(vv.begin()+j,vString.substr(i,t));

        //generating tBlocks for every substring and permutation combination
        for(int k=0; k<hv.size();k++){
            for(int l=0;l<perms.size();l++){
                for(int m=0;m<perms.size();m++){
                    tBlock block=tBlock(hv.at(k),vv.at(k),perms.at(l),perms.at(m));

                    //buidling a string key for the map
                    stringstream ss;
                    ss<<hv.at(k)<<vv.at(k);
                    string s;
                    for(auto const& e : perms.at(l)) s += to_string(e);                      
                    for(auto const& e : perms.at(m)) s += to_string(e);
                    ss<<s;
                    
                    //adding the tblock to the unordered_map
                    blockMap.insert( { ss.str(), block });
                }
            }        
        }
        j++;
        i=(++i)*(t-1);
    }
    
}

//recursive function that creates all permutations of possible offsets(0,-1,1) 
//and stores the result in vector<vector<int>> perm
void fourRussians::permutate(vector<int> posOffset,vector<int> prefix, int length){
    
    //Base case
    if (length == 1){
            for (int j = 0; j < 3; j++){
                vector<int> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posOffset.at(j));
                perms.push_back(tmp);
            }
            
        }
    else
        {
            // One by one add all vectors from posOffsets and  make a recursive call with length-1
            for (int i = 0; i < 3; i++){
                vector<int> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posOffset.at(i));
                permutate(posOffset, tmp ,length - 1);
            }
        }

}

