#include "fourRussians.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

fourRussians::fourRussians(int t, string hString, string vString){
    this->t=t;  
    this->hString=hString;
    this->vString=vString;
}
    //Method for generating tBlocks
    void fourRussians::generateTBlocks(string hString, string vString, int t){

        //creating permutations of offset vectors (size t)
        std::vector<int> posOffset { 0,-1,1 };
        std::vector<int> prefix;
        permutate(posOffset,prefix,t);
        
        //ako hocete isprobat ispis permutacija
        for(int i=0;i<perms.size();i++){
            for(int j=0;j<perms.at(i).size();j++){
                //cout<<perms.at(i).at(j);
            }
            //cout<<"\n";
        }
        cout<<perms.size();
        

        //creating t size vectors of input strings and generating tBlocks 
        int i=0;
        int j=0;
        std::vector<std::string> hv;
        std::vector<string> vv;
        while(i<hString.length()-(t-1)){
            hv.insert(hv.begin()+j,hString.substr(i,t));
            vv.insert(vv.begin()+j,vString.substr(i,t));

            //generating tBlocks for every substring and permutation combination




            j++;
            i++;
            i=i*(t-1);
        }

    }

    void fourRussians::permutate(vector<int> posOffset,vector<int> prefix, int length){
        
        //Base case
        if (length == 1){
                for (int j = 0; j < 3; j++){
                    std::vector<int> tmp;
                    if (prefix.size()!=0){
                        tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                    }
                    tmp.push_back(posOffset.at(j));
                    perms.push_back(tmp);
                }
                
            }
        else
            {
               // One by one add all vectors from posOffsets and recursively call for "length" equals to "length"-1
                for (int i = 0; i < 3; i++){
                    std::vector<int> tmp;
                    if (prefix.size()!=0){
                        tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                    }
                    tmp.push_back(posOffset.at(i));
                    permutate(posOffset, tmp ,length - 1);
                }
            }

    }

