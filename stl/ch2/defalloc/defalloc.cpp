//defalloc.cpp
#include "defalloc.h"
#include <vector>
#include <iostream>
using namespace std;
int main(){
    int ia[5] = {1,2,3,4,5};
    unsigned int i;
    vector<int,defalloc::allocator<int> > iv(ia,ia+5);
    for(i=0;i<iv.size();i++)
        cout << iv[i] << " ";
    cout << endl;
}