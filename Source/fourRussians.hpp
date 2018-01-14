#include <string>
#include <vector>
#include <unordered_map>


using namespace std;

class fourRussians{
    public:
        int t;
        string hString;
        string vString;
        std::vector<vector<int>> perms;
        

        fourRussians(int t, string hstring, string vstring); 

        void permutate(vector<int> posOffset,vector<int> prefix, int length);
        void generateTBlocks();
};

