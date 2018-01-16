#include "fourRussians.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "tBlock.hpp"
#include <unordered_map>
#include <thread>
#include <sstream>
#include <mutex>
#include <cmath>


using namespace std;

fourRussians::fourRussians(int t, string hString, string vString){
    this->t=t;  
    this->hString=hString;
    this->vString=vString;

    for(int i=0;i<hString.size();i+=t){
        hSubS.push_back(hString.substr(i,t));
    }
    for(int i=0;i<vString.size();i+=t){
        vSubS.push_back(vString.substr(i,t));
    }
        
}

string intVecToStr(vector<int>& vec){
    string s;
    for(int i=0;i<vec.size();i++){
        if (vec[i]==0) s+="0";
        if (vec[i]==1) s+="1";
        if (vec[i]==-1) s+="-1";
    }
    return s;
}

//Method for generating and storing tBlocks
void fourRussians::generateTBlocks(){

    int size= pow(4,t);
    permsSP= new string[size];

    //creating permutations of offset vectors (size t) 
    vector<int> posOffset { 0,-1,1 };
    vector<int> prefixO;
    permutateO(posOffset,prefixO,t);

    const char posString[]={ 'A','C','G','T' };
    int counter=0;
    int* k=&counter;
    permutateS(posString,"",4,t,k);
   
    

    //creating t size vectors of input strings and generating tBlocks 
    // int i=0;
    // int j=0;
    // vector<string> hv;
    // vector<string> vv;
    
    //creating vectors containing substrings of initial strings 
    // for(int i=0;i<vString.size();i+=t){
    //     hv.push_back(hString.substr(i,t));
    //     vv.push_back(vString.substr(i,t));
    // }
    
    //generating tBlocks for every substring and permutation combination
    // for(int k=0; k<hv.size();k++){
    //     for(int n=0;n<vv.size();n++){
    //         for(int l=0;l<perms.size();l++){
    //            for(int m=0;m<perms.size();m++){
    //                 tBlock block=tBlock(hv.at(k),vv.at(n),perms.at(l),perms.at(m));

    //                 //buidling a string key for the map
    //                 stringstream ss;
    //                 ss<<hv.at(k)<<vv.at(n);
    //                 string s;
    //                 for(auto const& e : perms.at(l)) s += to_string(e);                      
    //                 for(auto const& e : perms.at(m)) s += to_string(e);
    //                 ss<<s;
                        
    //                 //adding the tblock to the unordered_map
    //                 blockMap.insert( { ss.str(), block });
    //             }
    //         } 
    //     }       
    // }


    vector<thread> workers;
    mutex semafor;
    auto thr=[this,&semafor,size](pair<vector<int>,string> el2){
        for(pair<vector<int>,string> el1 : permsO){
            for(int j=0;j<size;j++){
               for(int k=0;k<size;k++){
                    string x=*(permsSP+j);
                    string y=*(permsSP+k);
                    tBlock block=tBlock(x,y,el2.first,el1.first);

                    //buidling a string key for the map
                    //stringstream ss;
                    //ss<<x<<y;
                    //string s;
                    // for(auto const& e : b) s += to_string(e);                      
                    // for(auto const& e : permsO[i]) s += to_string(e);
                    //s+=intVecToStr(b);
                    //s+=intVecToStr(permsO[i]);
                    //ss<<s;
                        
                    //adding the tblock to the unordered_map
                    semafor.lock();
                    string s=x+y+el2.second+el1.second;
                    blockMap.insert( { s, block });
                    //blockMap.insert( { ss.str(), block });
                    semafor.unlock();
                }
            } 
        }  
    };
    for(pair<vector<int>,string> element : permsO){
        workers.push_back(thread(thr,element));
    }
    

    /*
    for(int i=0; i<permsO.size();i++){
        // string tmp(permsS[i].begin(),permsS[i].end());
        workers.push_back(thread(thr,permsO[i]));
    }
    */
    cout<<workers.size()<<endl;
    for(auto &t :workers){
        t.join();
    }
    //cout<<blockMap.size();

    delete[] permsSP;
    
}

//recursive function that creates all permutations of possible offsets(0,-1,1) 
//and stores the result in vector<vector<int>> perm
void fourRussians::permutateO(vector<int> posOffset,vector<int> prefix, int length){
    
    //Base case
    if (length == 1){
            for (int j = 0; j < posOffset.size(); j++){
                vector<int> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posOffset.at(j));
                permsO.insert({tmp,intVecToStr(tmp)});
            }
    }
    else
        {
            // One by one add all vectors from posOffsets and  make a recursive call with length-1
            for (int i = 0; i < posOffset.size(); i++){
                vector<int> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posOffset.at(i));
                permutateO(posOffset, tmp ,length - 1);
            }
        }
}

/*
void fourRussians::permutateS(vector<char> posString,vector<char> prefix, int length){
    
    //Base case
    if (length == 1){
            for (int j = 0; j < posString.size(); j++){
                vector<char> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posString.at(j));
                permsSP.push_back(tmp);
            }
        }
    else
        {
            // One by one add all vectors from posString and  make a recursive call with length-1
            for (int i = 0; i < posString.size(); i++){
                vector<char> tmp;
                if (prefix.size()!=0){
                    tmp.insert( tmp.end(), prefix.begin(), prefix.end() );
                }
                tmp.push_back(posString.at(i));
                permutateS(posString, tmp ,length - 1);
            }
        }
}*/

void fourRussians::permutateS(const char str[],string prefix,const int n, const int lenght, int* k)
{
        if (lenght == 1)
            {
                for (int j = 0; j < n; j++){ 
                    *(permsSP + *k)=prefix + str[j];
                    (*k)++;
                }
            }
        else
            {
                for (int i = 0; i < n; i++){
                    permutateS(str, prefix + str[i], n, lenght - 1, k);
                
                }
            }
}


//Method for filling the d-table with precomputed values of t-blocks
/*void fourRussians::fillDTable(){
    //init d-table
    int n=hString.size()+1;
    dTable.resize(n);
    for(int i=0;i<dTable.size();i++){
        dTable[i].resize(n);
    }
    dTable[0][0]=0;
    for(int i=1;i<n;i++){
        dTable[0][i]=dTable[0][i-1]+1;
        dTable[i][0]=dTable[i-1][0]+1;
    }

    //for each substring and its offset get the precomputed t-block and fill the d-table with its output offsets
    for(int i=0;i<vString.size();i+=t){
        for(int j=0;j<hString.size();j+=t){
            string s1=hString.substr(j,t),s2=vString.substr(i,t),o1="",o2="";
            for(int k=1;k<t+1;k++){
                o1+=to_string(dTable[i][j+k]-dTable[i][j+k-1]);
                o2+=to_string(dTable[i+k][j]-dTable[i+k-1][j]);
            }
            tBlock currentBlock=blockMap[s1+s2+o1+o2];
            for(int k=1;k<t;k++){
                dTable[i+t][j+k]=dTable[i+t][j+k-1]+currentBlock.hOffsets[k-1];
                dTable[i+k][j+t]=dTable[i+k-1][j+t]+currentBlock.vOffsets[k-1];
            }
            dTable[i+t][j+t]=dTable[i+t][j+t-1]+currentBlock.hOffsets[currentBlock.hOffsets.size()-1];
        }
    }
}

//Print contents of d-table on stdout
void fourRussians::printDTable(){
    for(int i=0;i<dTable.size();i++){
        string s="";
        for(int j=0;j<dTable[i].size();j++){
            s+=to_string(dTable[i][j])+" ";
        }
        cout<<s<<endl;
    }
}
*/



void fourRussians::fillDTable(){
    // vector<string> hv;
    // vector<string> vv;
    // dTable.resize(vString.size()/t);

    // for(int i=0;i<dTable.size();i++){
    //     dTable[i].resize(hString.size()/t);
    // }
    //creating substrings of initial strings
    //there are two for loops in case string aren't the same length 
    // for(int i=0;i<hString.size();i+=t){
    //     hv.push_back(hString.substr(i,t));
    // }

    // for(int i=0;i<vString.size();i+=t){
    //     vv.push_back(vString.substr(i,t));
    // }


    //for every substring calculate tBlock and fill D-table
    tBlock currentBlock;
    for(int i=0;i<vSubS.size();i++){
        vector<tBlock> tmp;
        for(int j=0;j<hSubS.size();j++){
            string b="";
            string c="";
            for(int k=0;k<t;k++){
                    b+="1";
                    c+="1";
            }
            if(i==0 & j!=0){
                c="";
                // for(auto const& e : dTable[i][j-1].vOffsets) c += to_string(e);
                // for(auto const& e : currentBlock.vOffsets) c+=to_string(e);
                //c=intVecToStr(currentBlock.vOffsets);
                c=permsO[currentBlock.vOffsets];
            }
            if(i!=0 & j==0){
                b="";
                // for(auto const& e : blockMap[dTable[i-1][j]].hOffsets) b += to_string(e);
                //b=intVecToStr(blockMap[dTable[i-1][j]].hOffsets);
                b=permsO[dTable[i-1][j].hOffsets];
            }
            if(i!=0 & j!=0){
                c="";b="";
                // for(auto const& e : blockMap[dTable[i-1][j]].hOffsets) b += to_string(e);
                //b=intVecToStr(blockMap[dTable[i-1][j]].hOffsets);
                b=permsO[dTable[i-1][j].hOffsets];
                // for(auto const& e : dTable[i][j-1].vOffsets) c += to_string(e);
                // for(auto const& e : currentBlock.vOffsets) c+=to_string(e);
                //c=intVecToStr(currentBlock.vOffsets);
                c=permsO[currentBlock.vOffsets];
            }
            string s=hSubS[j]+vSubS[i]+b+c;
            //cout<<s;
            currentBlock=blockMap[s];
            //cout<<"\n";
            
            tmp.push_back(currentBlock);
        }
        dTable.push_back(tmp);
    }

    // printDTable();

    
}


void fourRussians::printDTable(){
    // for(int i=0; i<dTable.size();i++){
    //     for(int j=0;j<dTable.size();j++){
    //         string s="";
    //         for(auto const& e : dTable[i][j].vOffsets) s += to_string(e);
    //         cout<<s;
    //     }
    // }
}


//Get min string edit distance
int fourRussians::getMinDistance(){
    int tmp=0;
    for(int i=0;i<dTable.back().size();i++){
        for(auto &n:dTable.back()[i].hOffsets){
            tmp+=n;
        }
    }
    return tmp+hString.size();
    //return dTable[n][n];
}

