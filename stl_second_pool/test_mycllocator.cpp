#include "second_allocator.h"
#include <vector>
using namespace std;

int main()
{
    vector<int, second_allocator<int>> vec;
    
    for(int i = 0; i < 100; i++){
        vec.push_back(rand() % 1000);
    }
    for(int val : vec){
        cout << val << " ";
    }
    return 0;
} 