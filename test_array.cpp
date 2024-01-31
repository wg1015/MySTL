#include<Array.hpp>
#include<array>
#include<iostream>

using namespace std;
template<size_t N>
ostream& operator<<(ostream& os, array<int,N> arr){
    for(size_t i = 0 ; i < arr.size() ; i++){
        os<<arr[i]<<" ";
    }
    os<<endl;
    return os;
}

using namespace std;
template<size_t N>
ostream& operator<<(ostream& os, Array<int,N> arr){
    for(size_t i = 0 ; i < arr.size() ; i++){
        os<<arr[i]<<" ";
    }
    os<<endl;
    return os;
}


void foo(array<int,2> a){
    cout<<a.size()<<endl;
}
