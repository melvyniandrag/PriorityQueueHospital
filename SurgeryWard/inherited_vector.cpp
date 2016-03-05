/*
 * A code sample showing the use of the STL (priority_queue, vector), inheritence from an
 * abstract class, templated classes, and templated functors.
 * Also, use of c++11 features.
 * Also, use of pointers to store inherited elements in a vector of type base_class.
 * 
 * There's alot of interesting C++ stuff in here.
 *
 * I tried to replace pointers with std and boost smart pointers, but it seems that they can't 
 * make a vector of base class smart pointers and fill that with inherited smart pointers. If you 
 * know how, please tell me. The code here uses raw pointers and works.
 * 
 * Melvyn Ian Drag ~ 1 March 2015. 
 * Compile with g++ -std=c++11 -o inherit inherited_vector.cpp
*/


#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class base{
public:
    virtual double show(){return 0;}
    virtual void store(double a){};
};

template<typename T, typename FUNC>
class inherited: public base{
    double test;
    FUNC func;
    priority_queue<T, vector<T>, FUNC> pq;
public:
    inherited(FUNC f) : func(f){}; 
    void add(vector<T> v){for(auto i : v) this->pq.push(i);} 
    T get(){return pq.top();}
    void store(double a){
        test = a;
    }
    double show(){
        return test;
    }
};

template<typename T>
struct IncrSort{
    bool operator()(T t1, T t2){
        return t1 > t2;
    }
};

template<typename T>
struct DecrSort{
    bool operator()(T t1, T t2){
        return t1 < t2;
    }
}; 

int main(){
    vector<double> v{1, 2, 5, 0, 3, 10, 4};
    
    vector<base*> vec;
    IncrSort<double> incr_sort;
    DecrSort<double> decr_sort;

    inherited<double, IncrSort<double> >* one = new inherited<double, IncrSort<double> >(incr_sort);
    one->add(v);
    vec.push_back(one);
    
    inherited<double, DecrSort<double> >* two = new inherited<double, DecrSort<double> >(decr_sort);
    two->add(v);
    vec.push_back(two);

    cout << one->get() << endl;
    cout << two->get() << endl;
    return 0;
} 
