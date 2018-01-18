
#pragma once
#include <string>
#include <vector>
#include <cmath>


using namespace std;

//authors = Iva
//this is custom implementation of hash function for vectors
struct VectorHash {
    size_t operator()(vector<int> v) const noexcept {
                int n=0;
                int counter=v.size();
                int minusOne=2;
                for(int i=0;i<v.size(); i++){
                    if(v.at(i)==-1)
                        n+=pow(10,counter-1)*minusOne;
                    else
                        n+=pow(10,counter-1)*v.at(i);
                    counter--;
                }
                return size_t(n);
             }
    };
