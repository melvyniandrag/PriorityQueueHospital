#include <iostream>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;

class comparator{
public:
    int x;
};

class int_comparator1 : public comparator{
public:
    bool operator()(int x1, int x2){
        return x1 > x2;
    }
};
class int_comparator2 : public comparator{
public:
    bool operator()(int x1, int x2){
        return x1 < x2;
    }
};


template <typename T, typename FUNC>
class demo2 {
  FUNC func;  // declares 'func' to be of type FUNC (a template parameter)
       
public:
  demo2( FUNC f ) : func( f ) {}
  void Add2Q(vector<T> v){
       for (auto i : v)
       {
            this->pq.push(i);
       }
   } 
   T GetTop(){
        T top = this->pq.top();
        this->pq.pop();
        return top;
    }
   priority_queue<T, vector<T>, FUNC> pq;
};


int main() {
  vector<int> v{1, 2, 5, 0, 3, 10, 4};
  vector< demo2<int, comparator> > demo_vec;

  int_comparator1 ic1;
  demo2 <int, int_comparator1> demo2_instance1(ic1);
  demo2_instance1.Add2Q(v);
  
  int_comparator2 ic2;
  demo2 <int, int_comparator2> demo2_instance2(ic2);
  demo2_instance2.Add2Q(v);
 
    demo_vec.push_back(demo2_instance1);
    demo_vec.push_back(demo2_instance2);

    return 0;
}
