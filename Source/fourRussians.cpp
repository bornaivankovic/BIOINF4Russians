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
#include <atomic>
#include <condition_variable>


using namespace std;

fourRussians::fourRussians(int t, string hString, string vString, string outputFile){
    this->t=t;
    for(int i=hString.size()%t;i>0;i--){
        hString.erase(hString.size()-1,1);
    }
    for(int i=vString.size()%t;i>0;i--){
        vString.erase(vString.size()-1,1);
    }
    this->hString=hString;
    this->vString=vString;
    this->outputFile=outputFile;
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
   
        
    vector<thread> workers;
    mutex semafor;
    auto thr=[this,&semafor,size](pair<vector<int>,string> el2){
        for(pair<vector<int>,string> el1 : permsO){
            for(int j=0;j<size;j++){
               for(int k=0;k<size;k++){
                    string x=*(permsSP+j);
                    string y=*(permsSP+k);
                    tBlock block=tBlock(x,y,el2.first,el1.first);

                    //adding the tblock to the unordered_map
                    semafor.lock();
                    string s=x+y+el2.second+el1.second;
                    blockMap.insert( { s, block });
                    semafor.unlock();
                }
            } 
        }  
    };
    for(pair<vector<int>,string> element : permsO){
        workers.push_back(thread(thr,element));
    }
    

    
    //cout<<workers.size()<<endl;
    for(auto &t :workers){
        t.join();
    }

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


void fourRussians::fillDTable(){
    dTable.resize(vString.size()/t);
    for(int i=0;i<dTable.size();i++){
        dTable[i].resize(hString.size()/t);
    }

    for(int i=0;i<dTable.size();i++){
        for(int j=0;j<dTable.size();j++){
            string x=hSubS[j],y=vSubS[i],b,c;
            if(i==0){
                b=string(t,'1');
            }
            else{
                b=permsO[dTable[i-1][j]->hOffsets];
            }
            if(j==0){
                c=string(t,'1');
            }
            else{
                c=permsO[dTable[i][j-1]->vOffsets];
            }
            dTable[i][j]=&blockMap[x+y+b+c];
        }
    }

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

void fourRussians::makeEditScript(){
    int i=t,j=t,x=vSubS.size()-1,y=hSubS.size()-1;

    //v1 - diagonal value, v2 - left value, v3 - up value
    int v1,v2,v3,m;

    //0=diag 1=left 2=up
    vector<int> actions;
    vector<vector<int>> curBlock;
    vector<int> b,c;
    while(true){
        if(x==0 && y==0 && i==0 && j==0 ) break;
        if(x==0){
            b=vector<int>(t,1);
        }
        else{
            b=dTable[x-1][y]->hOffsets;
        }
        if(y==0){
            c=vector<int>(t,1);
        }
        else{
            c=dTable[x][y-1]->vOffsets;
        }
        
        curBlock=tBlock::calcBody(t,hSubS[y],vSubS[x],b,c);
        if(x<=0 || y<=0){
            if(x<=0) x=0;
            if(y<=0) y=0;
            if(i-1<0 ){
                v3=hString.size();
            }
            else{
                v3=curBlock[i-1][j];
            }
            if(j-1<0 ){
                v2=hString.size();
            }
            else{
                v2=curBlock[i][j-1];
            }
            if(i-1<0 || j-1<0){
                v1=hString.size();
            }
            else{
                v1=curBlock[i-1][j-1];
            }
        }
        else{
            v1=curBlock[i-1][j-1];
            v2=curBlock[i][j-1];
            v3=curBlock[i-1][j];
        }
        m=min({v1,v2,v3});
        // cout<<x<<","<<y<<","<<i<<","<<j<<":"<<v1<<","<<v2<<","<<v3<<endl;
        if(m==v1){
            //go diag
            i--;
            j--;
            actions.push_back(0);
            if(i==0 && j>0){
                if(x!=0){
                    x--;
                    i=t;
                }
            }
            if(j==0 && i>0){
                if(y!=0){
                    y--;
                    j=t;
                }
            }
            if(j==0 && i==0){
                if(x!=0 && y!=0){
                    x--;
                    y--;
                    i=j=t;
                }
            }
        }
        else if(m==v2){
            //go left
            j--;
            actions.push_back(1);
            if(j==0){
                if(y!=0){
                    y--;
                    j=t;
                }
            }
        }
        else{
            //go up
            i--;
            actions.push_back(2);
            if(i==0){
                if(x!=0){
                    x--;
                    i=t;
                }
            }
        }
    }
    string fs="",ss="",mid="";
    int asdf=0;
    int s1=0,s2=0;
    for(int i=actions.size()-1;i>=0;i--){
        if(actions[i]==0){
            if (hString[s1]==vString[s2]){
                mid+="|";
            }
            else{
                mid+=".";
            }
            fs+=hString[s1++];
            ss+=vString[s2++];
        }
        else if(actions[i]==1){
            mid+=" ";
            fs+=hString[s1++];
            ss+="-";
        }
        else{
            mid+=" ";
            fs+="-";
            ss+=vString[s2++];
        }
    }
    cout<<fs<<endl;
    cout<<mid<<endl;
    cout<<ss<<endl;
}

//Get min string edit distance
int fourRussians::getMinDistance(){
    int tmp=0;
    for(int i=0;i<dTable.back().size();i++){
        for(auto &n:dTable.back()[i]->hOffsets){
            tmp+=n;
        }
    }
    minDistance = tmp + hString.size();
    return tmp+hString.size();
}

