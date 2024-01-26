#include<Vector.hpp>
#include<iostream>
using namespace std;

template<typename params>
ostream& operator<<(ostream& os,params& vector){
    for(auto elem:vector){
        os<<elem;
    }
    return os;
}



int main(){
    Vector<int> arr{1,2,3};
    
    cout<<arr<<endl;

    
    Vector<int> crr(4,3);
    crr.insert(crr.end(),4);
    crr.push_back(5);
    cout<<crr<<endl;
    crr.erase(crr.begin()+1,crr.begin()+3);
    cout<<crr<<endl;

    return 0;
}