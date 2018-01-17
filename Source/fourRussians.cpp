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




class SyncObj {
    mutex mux;
    condition_variable cv;  
    bool completed[2]{ true,true };

public:
    void signalCompetionT1T2(int id) {
        lock_guard<mutex> ul(mux);
        completed[id] = true;
        cv.notify_all();
    }
    void signalCompetionT3() {
        lock_guard<mutex> ul(mux);
        completed[0] = false;
        completed[1] = false;
        cv.notify_all();
    }
    void waitForCompetionT1T2() {
        unique_lock<mutex> ul(mux);             
        cv.wait(ul, [&]() {return completed[0] && completed[1]; });         
    }
    void waitForCompetionT3(int id) {
        unique_lock<mutex> ul(mux);         
        cv.wait(ul, [&]() {return !completed[id]; });           
    }       
};


void fourRussians::fillDTable(){
    dTable.resize(vString.size()/t);
    for(int i=0;i<dTable.size();i++){
        dTable[i].resize(hString.size()/t);
    }

    SyncObj obj;
    mutex l;
    auto thrDiag=[this,&obj,&l](){
        for(int i=0;i<vSubS.size();i++){
            obj.waitForCompetionT1T2();
            string x=hSubS[i],y=vSubS[i],b,c;
            if(i==0){
                b=c=string(t,'1');
            }
            else{
                b=permsO[dTable[i-1][i]->hOffsets];
                c=permsO[dTable[i][i-1]->vOffsets];
            }
            dTable[i][i]=&blockMap[x+y+b+c];
            obj.signalCompetionT3();
        }
    };
    auto thrRow=[this,&obj,&l](){
        for(int i=0;i<vSubS.size();i++){
            obj.waitForCompetionT3(0);
            tBlock* prevBlock=dTable[i][i];
            for(int j=i+1;j<hSubS.size();j++){
                string x=hSubS[j],y=vSubS[i],b,c=permsO[prevBlock->vOffsets];
                if(i==0){
                    b=string(t,'1');
                }
                else{
                    b=permsO[dTable[i-1][j]->hOffsets];
                }
                dTable[i][j]=&blockMap[x+y+b+c];
                prevBlock=dTable[i][j];
            }
            obj.signalCompetionT1T2(0);
        }
    };
    auto thrCol=[this,&obj,&l](){
        for(int i=0;i<hSubS.size();i++){
            obj.waitForCompetionT3(1);
            tBlock *prevBlock=dTable[i][i];
            for(int j=i+1;j<vSubS.size();j++){
                string x=hSubS[i],y=vSubS[j],b=permsO[prevBlock->hOffsets],c;
                if(i==0){
                    c=string(t,'1');
                }
                else{
                    c=permsO[dTable[j][i-1]->vOffsets];
                }
                dTable[j][i]=&blockMap[x+y+b+c];
                prevBlock=dTable[j][i];
            }
            obj.signalCompetionT1T2(1);
        }
    };

    vector<thread> threads;
    threads.push_back(thread(thrDiag));
    threads.push_back(thread(thrRow));
    threads.push_back(thread(thrCol));
    for(auto & t:threads){
        t.join();
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


//Get min string edit distance
int fourRussians::getMinDistance(){
    int tmp=0;
    for(int i=0;i<dTable.back().size();i++){
        for(auto &n:dTable.back()[i]->hOffsets){
            tmp+=n;
        }
    }
    return tmp+hString.size();
}

